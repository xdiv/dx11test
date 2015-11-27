#include "pch.h"
#include "ShaderBase.h"
#include "ShaderModel.h"

ShaderBase::ShaderBase(ID3D* d3d)
	:	m_d3d(), m_VS(), m_PS(), 
	m_Layout(), m_SampleState()
{
	m_d3d = d3d;
}

ShaderBase::~ShaderBase()
{
	Release();
}

void ShaderBase::Release()
{
	SAFE_RELEASE(m_SampleState);
	SAFE_RELEASE(m_Layout);
	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_PS);
}

void ShaderBase::Init(WCHAR* vsFilename, WCHAR* psFilename, D3D11_INPUT_ELEMENT_DESC * iedf, int desc_count)
{
	HRESULT result;
	ID3DBlob *VS, *PS, *errorMessage;

	result = D3DCompileFromFile(vsFilename, 0, 0, "VShader", "vs_5_0", 0, 0, &VS, &errorMessage);
	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, vsFilename);
	}

	result = D3DCompileFromFile(psFilename, 0, 0, "PShader", "ps_5_0", 0, 0, &PS, &errorMessage);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, psFilename);
	}

	m_d3d->GetD3DDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &m_VS);
	m_d3d->GetD3DDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &m_PS);

	m_d3d->GetD3DDevice()->CreateInputLayout(iedf, desc_count, VS->GetBufferPointer(), VS->GetBufferSize(), &m_Layout);

	SAFE_RELEASE(VS);
	SAFE_RELEASE(PS);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferTypeB);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	m_d3d->GetD3DDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &m_IVsBuffer);
}

void ShaderBase::CreatePixelShaderBuffer()
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

	result = m_d3d->GetD3DDevice()->CreateBuffer(&bufferDesc, nullptr, &m_IPxBuffer);       // create the buffer
}

void ShaderBase::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
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
	MessageBox(m_d3d->GetHwnd(), L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void ShaderBase::CreateSampler()
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

	m_d3d->GetD3DDevice()->CreateSamplerState(&samplerDesc, &m_SampleState);
}

void ShaderBase::SetVertexShaderBuffers(XMMATRIX * data)
{
	//	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferTypeB* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	data = &XMMatrixTranspose(*data);

	// Lock the constant buffer so it can be written to.
	m_d3d->GetD3DDeviceContext()->Map(m_IVsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferTypeB*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = *data;

	// Unlock the constant buffer.
	m_d3d->GetD3DDeviceContext()->Unmap(m_IVsBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_d3d->GetD3DDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_IVsBuffer);
}

void ShaderBase::SetPixelShaderBuffers(PSConstBuffer * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PSConstBuffer* dataPtr;
	m_d3d->GetD3DDeviceContext()->Map(m_IPxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the transparent constant buffer.
	dataPtr = (PSConstBuffer*)mappedResource.pData;

	dataPtr->color = data->color;
	dataPtr->transperency = data->transperency;
	// Copy into the constant buffer.
	dataPtr->hasTexture = data->hasTexture;
	dataPtr->hasColor = data->hasColor;

	// Unlock the buffer.
	m_d3d->GetD3DDeviceContext()->Unmap(m_IPxBuffer, 0);

	// Now set the texture translation constant buffer in the pixel shader with the updated values.
	m_d3d->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, &m_IPxBuffer);
}