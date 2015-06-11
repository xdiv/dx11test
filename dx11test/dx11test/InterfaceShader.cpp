#include "InterfaceShader.h"


InterfaceShader::InterfaceShader()
{
	vert_count = 4;
	indexCount = 6;
	pVBuffer = 0;
	pIBuffer = 0;
}


InterfaceShader::~InterfaceShader()
{
	ShutDown();
}

void InterfaceShader::ShutDown()
{
	SAFE_RELEASE(pVBuffer);
	SAFE_RELEASE(pIBuffer);
}

void InterfaceShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderBase::Init(dev, hWnd, L"2dVertShader.hlsl", L"2dPixShader.hlsl", devcon, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	ShaderBase::CreatePixelShaderBuffer(dev);

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;

	mesh2d mesh[] =
	{
		mesh2d(0, 0, 0, 0, 1),
		mesh2d(0, 1, 0, 0, 1),
		mesh2d(1, 1, 0, 0, 1),
		mesh2d(1, 0, 0, 0, 1)
	};

	UINT index[] = {0, 1, 2, 2, 3, 0 };

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = sizeof(mesh2d) * vert_count;             // size is the TEXVERTEX struct * 3
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	data.pSysMem = mesh;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	dev->CreateBuffer(&bufferDesc, &data, &pVBuffer);       // create the buffer

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	data.pSysMem = index;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	dev->CreateBuffer(&bufferDesc, &data, &pIBuffer);
}

void InterfaceShader::Render(ID3D11DeviceContext* devcon, float4 cube, ID3D11ShaderResourceView* texture, PSConstBuffer settings)
{
	UINT stride = sizeof(mesh2d), offset=0;

	//BuildSquare(devcon, cube);

	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ShaderBase::SetShaderParameters(devcon, settings);

	if (texture)
		devcon->PSSetShaderResources(0, 1, &texture);

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);
	devcon->Draw(indexCount, 0);
}

void InterfaceShader::BuildSquare(ID3D11DeviceContext* devcon, float4 cube)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mesh2d mesh[] =
	{
		mesh2d(cube.x, cube.y, 0, 0, 0),
		mesh2d(cube.x + cube.z, cube.y, 0, 0, 1),
		mesh2d(cube.x + cube.z, cube.y + cube.w, 0, 1, 1),
		mesh2d(cube.x, cube.y + cube.w, 0, 1, 0)
	};

	//UINT index[] = {0, 3, 2, 0, 2, 1};

	devcon->Map(pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // lock the instance buffer        
	memcpy((mesh2d*)mappedResource.pData, mesh, sizeof(mesh2d) * vert_count); //overwrite instance buffer with new data
	devcon->Unmap(pVBuffer, 0);
}