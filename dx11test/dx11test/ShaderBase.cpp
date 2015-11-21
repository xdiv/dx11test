#include "ShaderBase.h"

ShaderBase::ShaderBase()
{
	pVS = 0;    // the vertex shader
	pPS = 0;     // the pixel shader
	pLayout = 0;    // global
	IVsBuffer = 0;
	IPxBuffer = 0;
	pSampleState = 0;
}

ShaderBase::~ShaderBase()
{
	Release();
}

void ShaderBase::Release()
{
	SAFE_RELEASE(pSampleState);
	SAFE_RELEASE(pLayout);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(IVsBuffer);
	SAFE_RELEASE(IPxBuffer);
}

void ShaderBase::Init(ID3D11Device* dev, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, ID3D11DeviceContext* devcon, D3D11_INPUT_ELEMENT_DESC * iedf, int desc_count)
{
	HRESULT result;
	ID3DBlob *VS, *PS, *errorMessage;

	result = D3DCompileFromFile(vsFilename, 0, 0, "VShader", "vs_5_0", 0, 0, &VS, &errorMessage);
	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
	}

	result = D3DCompileFromFile(psFilename, 0, 0, "PShader", "ps_5_0", 0, 0, &PS, &errorMessage);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
	}

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pPS);

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

	dev->CreateBuffer(&matrixBufferDesc, nullptr, &IVsBuffer);
}

void ShaderBase::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(pVS, nullptr, 0);
	deviceContext->PSSetShader(pPS, nullptr, 0);

	// Set the sampler state in the pixel shader.
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
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

	result = dev->CreateBuffer(&bufferDesc, nullptr, &IPxBuffer);       // create the buffer
}

//void ShaderBase::SetShaderParameters(ID3D11DeviceContext* devcon, PSConstBuffer buffer)
//{
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	PSConstBuffer* dataPtr;
//	devcon->Map(IPxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//	// Get a pointer to the data in the transparent constant buffer.
//	dataPtr = (PSConstBuffer*)mappedResource.pData;
//
//	dataPtr->color = buffer.color;
//	dataPtr->transperency = buffer.transperency;
//	// Copy into the constant buffer.
//	dataPtr->hasTexture = buffer.hasTexture;
//	dataPtr->hasColor = buffer.hasColor;
//
//	// Unlock the buffer.
//	devcon->Unmap(IPxBuffer, 0);
//
//	// Now set the texture translation constant buffer in the pixel shader with the updated values.
//	devcon->PSSetConstantBuffers(0, 1, &IPxBuffer);
//}
//
//void ShaderBase::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix)
//{
////	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	MatrixBufferTypeB* dataPtr;
//	unsigned int bufferNumber;
//
//	// Transpose the matrices to prepare them for the shader.
//	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
//
//	// Lock the constant buffer so it can be written to.
//	deviceContext->Map(IVsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//	// Get a pointer to the data in the constant buffer.
//	dataPtr = (MatrixBufferTypeB*)mappedResource.pData;
//
//	// Copy the matrices into the constant buffer.
//	dataPtr->world = worldMatrix;
//
//	// Unlock the constant buffer.
//	deviceContext->Unmap(IVsBuffer, 0);
//
//	// Set the position of the constant buffer in the vertex shader.
//	bufferNumber = 0;
//
//	// Finanly set the constant buffer in the vertex shader with the updated values.
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &IVsBuffer);
//}

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

void ShaderBase::SetVertexShaderBuffers(ID3D11DeviceContext* devcon, XMMATRIX * data)
{
	//	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferTypeB* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	data = &XMMatrixTranspose(*data);

	// Lock the constant buffer so it can be written to.
	devcon->Map(IVsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferTypeB*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = *data;

	// Unlock the constant buffer.
	devcon->Unmap(IVsBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &IVsBuffer);
}

void ShaderBase::SetPixelShaderBuffers(ID3D11DeviceContext* devcon, PSConstBuffer * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PSConstBuffer* dataPtr;
	devcon->Map(IPxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the transparent constant buffer.
	dataPtr = (PSConstBuffer*)mappedResource.pData;

	dataPtr->color = data->color;
	dataPtr->transperency = data->transperency;
	// Copy into the constant buffer.
	dataPtr->hasTexture = data->hasTexture;
	dataPtr->hasColor = data->hasColor;

	// Unlock the buffer.
	devcon->Unmap(IPxBuffer, 0);

	// Now set the texture translation constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(0, 1, &IPxBuffer);
}

void ShaderBase::RenderIIT(ID3D11DeviceContext* devcon, XMMATRIX &worldMatrix, UINT indexCount, UINT instanceCount)
{
	SetVertexShaderBuffers(devcon, &worldMatrix);
	//devcon->PSSetShaderResources(0, 1, &texture); perkelti i modelRenderer

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, nullptr, 0);
	devcon->PSSetShader(pPS, nullptr, 0);

	devcon->PSSetSamplers(0, 1, &pSampleState);

	devcon->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

ID3D11Buffer* CreateD3D11Buffer(ID3D11Device* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags, void * data, UINT sysMemPitch, UINT sysMemSlicePitch)
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
	subData.SysMemPitch = sysMemPitch;
	subData.SysMemSlicePitch = sysMemSlicePitch;

	//delete mesh;

	dev->CreateBuffer(&bufferDesc, &subData, &buffer);       // create the buffer
	return buffer;
}

ID3D11ShaderResourceView* CreateD3D11TextureResourceView(ID3D11Device* dev, D3D11_USAGE usage, UINT width, UINT height, D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuAccesFlags, void * data, UINT sysMemPitch, UINT sysMemSlicePitch)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc; //textureDesc
	ZeroMemory(&desc, sizeof(desc));
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc; //srvDesc
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	ID3D11Texture2D* tex = nullptr; //m_texture
	ID3D11ShaderResourceView *textureView = nullptr; //m_textureView
	const UINT TexturePixelSize = 4;
	

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccesFlags;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;
	initData.SysMemPitch = sizeof(UINT8) * sysMemPitch * 4;
	/*initData.SysMemPitch = sysMemPitch * 4;
	initData.SysMemSlicePitch = sysMemSlicePitch * 4;*/
	
	hr = dev->CreateTexture2D(&desc, &initData, &tex);
	
	//hr = dev->CreateTexture2D(&desc, nullptr, &tex);

	if (SUCCEEDED(hr) && tex != 0)
	{
		//if (textureView != 0)
		{
			
			SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = 1;

			hr = dev->CreateShaderResourceView(tex, &SRVDesc, &textureView);

			//tex->Release();
			//delete(tex);
			if (FAILED(hr))
			{
				return nullptr;
			}

			return textureView;
		}
	}
	else
	{
		//MessageBox()
		return nullptr;
	}
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

	dev->CreateBuffer(&bufferDesc, nullptr, &buffer);       // create the buffer
	return buffer;
}
