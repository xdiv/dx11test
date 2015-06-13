#pragma once
#include "ShaderBase.h"
#include "DXMath.h"

class InterfaceShader: protected ShaderBase
{
private:
	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer

	UINT vert_count;
	UINT indexCount;

private:
	void BuildSquare(ID3D11DeviceContext* devcon, float4 cube);
public:
	InterfaceShader() {};
	~InterfaceShader() {};

	void Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	void Render(ID3D11DeviceContext* deviceContext, float4 cube, ID3D11ShaderResourceView* texture, PSConstBuffer setings, D3DXMATRIX world);
};

