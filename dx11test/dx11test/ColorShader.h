#pragma once
#include "ShaderBase.h"

struct VERTEX
{
	D3DXVECTOR3 pos;      // position
	D3DXCOLOR Color;    // color
};


class ColorShader :
	protected ShaderBase
{
private:
	static ColorShader* _instance;

protected:
	ColorShader(ColorShader const&);              // Don't Implement
	void operator=(ColorShader const&); // Don't implement
	ColorShader();
public:
	
	~ColorShader();
	void Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	void Release();
	static ColorShader* GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	static void ShutDown();
};

