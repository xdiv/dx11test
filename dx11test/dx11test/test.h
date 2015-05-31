#pragma once
#include "colorShader.h"

class test
{
private:
	ID3D11Buffer *pVBuffer; //vertex buffer
	ID3D11Buffer *pIBuffer; //index buffer
	ColorShader * cs;
public:
	test(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext *devcon, D3DXVECTOR3&);
	void Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	~test();
};

