#include "TextureShader.h"


TextureShader::TextureShader()
{

}


TextureShader::~TextureShader()
{
}

void TextureShader::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SW", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WC", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderBase::Init(dev, hWnd, L"TextureShader.hlsl", L"TextureShader.hlsl", devcon, ied, 5);

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

void TextureShader::RenderShader(ID3D11DeviceContext* devcon, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	ShaderBase::SetShaderParameters(devcon, worldMatrix, viewMatrix, projectionMatrix);
	devcon->PSSetShaderResources(0, 1, &texture);

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->PSSetSamplers(0, 1, &pSampleState);

	//devcon->Draw(indexCount, 0);
	devcon->DrawIndexed(indexCount, 0, 0);
}

void TextureShader::RenderShader(ID3D11DeviceContext* devcon, int indexCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView* texture)
{
	ShaderBase::SetShaderParameters(devcon, worldMatrix);
	devcon->PSSetShaderResources(0, 1, &texture);

	devcon->IASetInputLayout(pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(pVS, NULL, 0);
	devcon->PSSetShader(pPS, NULL, 0);

	devcon->PSSetSamplers(0, 1, &pSampleState);

	//devcon->Draw(indexCount, 0);
	devcon->DrawIndexed(indexCount, 0, 0);
}

TextureShader* TextureShader::_instance = 0;
TextureShader* TextureShader::GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	if (_instance)
		return _instance;

	_instance = new TextureShader();
	_instance->Init(dev, hWnd, devcon);
	return _instance;
}

void TextureShader::ShutDown()
{
	if (_instance)
	{
		_instance->ShutDown();
		_instance = 0;
	}
}
