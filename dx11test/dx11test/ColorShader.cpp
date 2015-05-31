#include "ColorShader.h"


ColorShader::ColorShader() : ShaderBase()
{
}


ColorShader::~ColorShader()
{
	
}

void ColorShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ShaderBase::Init(dev, hWnd, L"ColorShader.hlsl", L"ColorShader.hlsl", devcon, ied, 2);
}

void ColorShader::RenderShader(ID3D11DeviceContext* devcon, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	ShaderBase::SetShaderParameters(devcon, worldMatrix, viewMatrix, projectionMatrix);

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);
	devcon->Draw(indexCount, 0);
}

void ColorShader::Release()
{

}

ColorShader* ColorShader::_instance = 0;
ColorShader* ColorShader::GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	if (_instance)
		return _instance;

	_instance = new ColorShader();
	_instance->Init(dev, hWnd, devcon);
	return _instance;
}

void ColorShader::ShutDown()
{
	if (_instance)
	{
		SAFE_DELETE(_instance);
	}

}
