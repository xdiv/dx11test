#include "ShaderBase.h"

ShaderBase::ShaderBase()
{
	pVS = 0;    // the vertex shader
	pPS = 0;     // the pixel shader
	pLayout = 0;    // global
	m_matrixBuffer = 0;
	IPxBuffer = 0;
	pSampleState = 0;
}

ShaderBase::~ShaderBase()
{
	Relese();
}

void ShaderBase::Relese()
{
	SAFE_RELEASE(pSampleState);
	SAFE_RELEASE(pLayout);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(IPxBuffer);
}

void ShaderBase::Init(ID3D11Device* dev, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, ID3D11DeviceContext* devcon, D3D11_INPUT_ELEMENT_DESC * iedf, int desc_count)
{
	HRESULT result;
	ID3D10Blob *VS, *PS, *errorMessage;

	result = D3DX11CompileFromFile(vsFilename, 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, &errorMessage, 0);
	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
	}

	result = D3DX11CompileFromFile(psFilename, 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, &errorMessage, 0);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
	}

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	dev->CreateInputLayout(iedf, desc_count, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);

	SAFE_RELEASE(VS);
	SAFE_RELEASE(PS);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferTypeB);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	dev->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
}

void ShaderBase::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(pVS, NULL, 0);
	deviceContext->PSSetShader(pPS, NULL, 0);

	// Set the sampler state in the pixel shader.
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void ShaderBase::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,	D3DXMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferTypeA* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferTypeA*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}

void ShaderBase::CreatePixelShaderBuffer(ID3D11Device* dev)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc;

	//ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = sizeof(PSConstBuffer);             // size is the TEXVERTEX struct * 3
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = dev->CreateBuffer(&bufferDesc, NULL, &IPxBuffer);       // create the buffer
}

void ShaderBase::SetShaderParameters(ID3D11DeviceContext* devcon, PSConstBuffer buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PSConstBuffer* dataPtr;
	devcon->Map(IPxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the transparent constant buffer.
	dataPtr = (PSConstBuffer*)mappedResource.pData;

	dataPtr->color = buffer.color;
	dataPtr->transperency = buffer.transperency;
	// Copy into the constant buffer.
	dataPtr->hasTexture = buffer.hasTexture;
	dataPtr->hasColor = buffer.hasColor;

	// Unlock the buffer.
	devcon->Unmap(IPxBuffer, 0);

	// Now set the texture translation constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(0, 1, &IPxBuffer);
}

void ShaderBase::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix)
{
//	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferTypeB* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferTypeB*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}

void ShaderBase::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

ID3D11Buffer* CreateD3D11Buffer(ID3D11Device* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags, void * data)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ID3D11Buffer * buffer;
	buffer = 0;

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

	bufferDesc.Usage = usage;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = byteWidth;						   // size sizeof(struct)*elementCount
	bufferDesc.BindFlags = bindFlags;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = cpuAccesFlags;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//vertexData.pSysMem = OurVertices;
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	//delete mesh;

	dev->CreateBuffer(&bufferDesc, &subData, &buffer);       // create the buffer
	return buffer;
}

ID3D11Buffer* CreateD3D11BufferEmpty(ID3D11Device* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags)
{
	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer * buffer;
	buffer = 0;

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = usage;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = byteWidth;						   // size sizeof(struct)*elementCount
	bufferDesc.BindFlags = bindFlags;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = cpuAccesFlags;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	dev->CreateBuffer(&bufferDesc, NULL, &buffer);       // create the buffer
	return buffer;
}

void ShaderBase::CreateSampler(ID3D11Device* dev)
{
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	dev->CreateSamplerState(&samplerDesc, &pSampleState);
}

void ShaderBase::RenderIIT(ID3D11DeviceContext* devcon, D3DXMATRIX worldMatrix, UINT indexCount, UINT instanceCount)
{
	SetShaderParameters(devcon, worldMatrix);
	//devcon->PSSetShaderResources(0, 1, &texture); perkelti i modelRenderer

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->PSSetSamplers(0, 1, &pSampleState);

	devcon->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}