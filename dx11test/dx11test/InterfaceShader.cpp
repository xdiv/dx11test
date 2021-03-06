#include "InterfaceShader.h"

void InterfaceShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderBase::Init(dev, hWnd, L"2dVertShader.hlsl", L"2dPixShader.hlsl", devcon, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	ShaderBase::CreatePixelShaderBuffer(dev);
}

void InterfaceShader::Render(ID3D11DeviceContext* devcon, float4 cube, ID3D11ShaderResourceView* texture, PSConstBuffer settings, D3DXMATRIX world)
{
	UINT stride = sizeof(mesh2d);
	UINT offset = 0;

	BuildSquare(devcon, cube);
	SetVertexShaderBuffers(devcon, &world);

	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	SetPixelShaderBuffers(devcon, &settings);

	if (texture)
		devcon->PSSetShaderResources(0, 1, &texture);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->IASetInputLayout(pLayout);

	devcon->DrawIndexed(indexCount, 0, 0);
}

void InterfaceShader::BuildSquare(ID3D11DeviceContext* devcon, float4 cube)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mesh2d mesh[] =
	{
		mesh2d(cube.x, cube.y, 0, 0, 0),
		mesh2d(cube.x, cube.y - cube.w, 0, 0, 1),
		mesh2d(cube.x + cube.z, cube.y - cube.w, 0, 1, 1),
		mesh2d(cube.x + cube.z, cube.y, 0, 1, 0),
	};

	devcon->Map(pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // lock the instance buffer        
	memcpy((mesh2d*)mappedResource.pData, mesh, sizeof(mesh2d) * vert_count); //overwrite instance buffer with new data
	devcon->Unmap(pVBuffer, 0);
}