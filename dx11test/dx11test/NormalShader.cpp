#include "pch.h"
#include "NormalShader.h"

void NormalShader::Init()
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

	ShaderBase::Init(L"NormalVertexShader.hlsl", L"NormalPixelShader.hlsl", ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC));
	ShaderBase::CreatePixelShaderBuffer();
}

void NormalShader::Render(DMBdata* data)
{

	ID3D11Buffer* bufferPointers[] = { data->pVBuffer, data->pInsBuffer };

	m_d3d->GetD3DDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, data->stride, data->offset);
	m_d3d->GetD3DDeviceContext()->IASetIndexBuffer(data->pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (data->texture)
	{
		m_d3d->GetD3DDeviceContext()->PSSetShaderResources(0, 1, &(data->texture));
		m_d3d->GetD3DDeviceContext()->PSSetSamplers(0, 1, &m_SampleState);
	}

	// Set the vertex and pixel shaders that will be used to render this triangle.
	m_d3d->GetD3DDeviceContext()->VSSetShader(m_VS, nullptr, 0);
	m_d3d->GetD3DDeviceContext()->PSSetShader(m_PS, nullptr, 0);

	m_d3d->GetD3DDeviceContext()->IASetInputLayout(m_Layout);

	m_d3d->GetD3DDeviceContext()->DrawIndexedInstanced(data->indexCount, data->instanceCount, 0, 0, 0);
	data->instanceCount = 0;
}