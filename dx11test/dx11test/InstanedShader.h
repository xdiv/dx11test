#pragma once

#include "ShaderBase.h"
#include "../cdxml/complex_types.h"

class InstanedShader: protected ShaderBase
{
private:
	static InstanedShader* _instance;

	//functions
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
public:
	InstanedShader(){};
	~InstanedShader(){};
	void Init(ID3D11Device*, HWND, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext* devcon, D3DXMATRIX worldMatrix, UINT indexCount, UINT instanceCount);
	static InstanedShader* GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	static void ShutDown();
};

