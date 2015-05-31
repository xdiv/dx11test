#include "ShaderBase.h"

ShaderBase::ShaderBase()
{
	pVS = 0;    // the vertex shader
	pPS = 0;     // the pixel shader
	pLayout = 0;    // global
}

ShaderBase::~ShaderBase()
{
	SAFE_RELEASE(pLayout);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(pPS);
}

void ShaderBase::Relese()
{

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
	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	//devcon->VSSetShader(pVS, 0, 0);
	///devcon->PSSetShader(pPS, 0, 0);
	

	dev->CreateInputLayout(iedf, desc_count, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	//devcon->IASetInputLayout(pLayout);

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
	HRESULT result;
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