#include "InstanedShader.h"


InstanedShader::InstanedShader()
{
	pVBuffer = 0;
	pIBuffer = 0;
	pInsBuffer = 0;
	texture = 0;
}

void InstanedShader::Init(ID3D11Device* dev, ID3D11DeviceContext* devcon, HWND hwnd)
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	ShaderBase::Init(dev, hwnd, L"VertexShader.hlsl", L"PixelShader.hlsl", devcon, ied, (sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC)));

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

InstanedShader::~InstanedShader()
{
	SAFE_DELETE(pVBuffer);
	SAFE_DELETE(pIBuffer);
	SAFE_DELETE(texture);
	SAFE_DELETE(pInsBuffer);
}

void InstanedShader::Prepare(ID3D11Device* dev)
{
	indexCount = 0;

	meshv2 mesh[] =
	{
		{ float3(-1, 15, 1), float2(1,0) },
		{ float3(-1, 15,-1), float2(0,0) },
		{ float3( 1, 15, 1), float2(1,1) },
		{ float3( 1, 15, -1), float2(0,1) },
	};

	meshSize = sizeof(mesh) / sizeof(meshv2);

	UINT indexLIst[] = { 0, 1, 3, 0, 3, 2 };

	indexCount = sizeof(indexLIst) / sizeof(UINT);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;

	float3 instancesList[] = { float3(-5, 1, 0), float3(5, 1, 0) };
	instanceCount = sizeof(instancesList) / sizeof(float3);

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	ZeroMemory(&vertexData, sizeof(vertexData));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	vertexBufferDesc.ByteWidth = sizeof(meshv1) * meshSize;             // size is the TEXVERTEX struct * 3
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = mesh;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	dev->CreateBuffer(&vertexBufferDesc, &vertexData, &pVBuffer);       // create the buffer

	ZeroMemory(&indexData, sizeof(indexData));
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indexLIst;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	dev->CreateBuffer(&indexBufferDesc, &indexData, &pIBuffer);

	ZeroMemory(&instanceData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&instanceBufferDesc, sizeof(D3D11_BUFFER_DESC));

	instanceData.pSysMem = instancesList; // initial data
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(float3) * instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //needed for Map/Unmap
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	dev->CreateBuffer(&instanceBufferDesc, &instanceData, &pInsBuffer);
	
	D3DX11CreateShaderResourceViewFromFile(dev, L"texttest.dds", NULL, NULL, &texture, NULL);
}

void InstanedShader::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferTypeB* dataPtr;

	UINT stride[] = { sizeof(meshv2), sizeof(float3) };
	UINT offset[] = { 0, 0};
	ID3D11Buffer * bufferPointers[] = { pVBuffer, pInsBuffer };

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix); // Transpose the matrices to prepare them for the shader.
	
	//matrix buffers
	devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the constant buffer so it can be written to.
	dataPtr = (MatrixBufferTypeB*)mappedResource.pData; // Get a pointer to the data in the constant buffer.
	dataPtr->world = worldMatrix; // Copy the matrices into the constant buffer.
	devcon->Unmap(m_matrixBuffer, 0);// Unlock the constant buffer.

	//instancing
	//float3 instancesList[] = {float3(1, 0, 1), float3(2, 0, 2), float3(3, 0, 3)};
	//instanceCount = sizeof(instancesList) / sizeof(float3);
	//devcon->Map(pInsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // lock the instance buffer        
	//memcpy((float3*)mappedResource.pData, instancesList, sizeof(float3) * instanceCount); //overwrite instance buffer with new data
	//devcon->Unmap(pInsBuffer, 0); // unlock the instanceBuffer

	devcon->VSSetConstantBuffers(0, 1, &m_matrixBuffer); // Finanly set the constant buffer in the vertex shader with the updated values.
	devcon->PSSetShaderResources(0, 1, &texture);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->PSSetSamplers(0, 1, &pSampleState);

	devcon->IASetInputLayout(pLayout);
	//devcon->Draw(indexCount, 0);

	devcon->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}
