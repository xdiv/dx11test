#include "NormalShader.h"

void NormalShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderBase::Init(dev, hWnd, L"2dVertShader.hlsl", L"2dPixShader.hlsl", devcon, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	ShaderBase::CreatePixelShaderBuffer(dev);
}

void NormalShader::Render(ID3D11DeviceContext* devcon, DMBdata* data, ID3D11ShaderResourceView* texture)
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
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->IASetInputLayout(pLayout);

	devcon->DrawIndexedInstanced(data->indexCount, data->instanceCount, 0, 0, 0);
}