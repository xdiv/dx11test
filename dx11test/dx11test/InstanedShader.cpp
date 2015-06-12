#include "InstanedShader.h"

void InstanedShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	1, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	2, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS",0, DXGI_FORMAT_R32G32B32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	ShaderBase::Init(dev, hWnd, L"VertexShader.hlsl", L"PixelShader.hlsl", devcon, ied, (sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC)));
	ShaderBase::CreateSampler(dev);
}

void InstanedShader::Render(ID3D11DeviceContext* devcon, D3DXMATRIX worldMatrix, UINT indexCount, UINT instanceCount)
{
	ShaderBase::RenderIIT(devcon, worldMatrix, indexCount, instanceCount);
}

InstanedShader* InstanedShader::_instance = 0;
InstanedShader* InstanedShader::GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	if (_instance)
		return _instance;

	_instance = new InstanedShader();
	_instance->Init(dev, hWnd, devcon);
	return _instance;
}

void InstanedShader::ShutDown()
{
	if (_instance)
	{
		_instance->ShutDown();
		_instance = 0;
	}
}