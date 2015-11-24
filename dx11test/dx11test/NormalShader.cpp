#include "pch.h"
#include "NormalShader.h"

void NormalShader::Init(ID3D11Device2* dev, HWND hWnd, ID3D11DeviceContext2 * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS",0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "POSITION",	1, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "POSITION",	2, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	ShaderBase::Init(dev, hWnd, L"NormalVertexShader.hlsl", L"NormalPixelShader.hlsl", devcon, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	ShaderBase::CreatePixelShaderBuffer(dev);
}

void NormalShader::Render(ID3D11DeviceContext2* devcon, DMBdata* data)
{

	ID3D11Buffer* bufferPointers[] = { data->pVBuffer, data->pInsBuffer };

	devcon->IASetVertexBuffers(0, 2, bufferPointers, data->stride, data->offset);
	devcon->IASetIndexBuffer(data->pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (data->texture)
	{
		devcon->PSSetShaderResources(0, 1, &(data->texture));
		devcon->PSSetSamplers(0, 1, &pSampleState);
	}

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, nullptr, 0);
	devcon->PSSetShader(pPS, nullptr, 0);

	devcon->IASetInputLayout(pLayout);

	devcon->DrawIndexedInstanced(data->indexCount, data->instanceCount, 0, 0, 0);
	data->instanceCount = 0;
}