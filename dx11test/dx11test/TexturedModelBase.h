#pragma once
#include "TextureShader.h"
#include "PublicData.h"
#include "MD5Model.h"
#include "MD5ModelBinary.h"

class TexturedModelBase
{
private:
	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer
	ID3D11ShaderResourceView* texture;
	TextureShader* tsinst;

	unsigned long vert_count;
	unsigned long indexCount;
public:
	TexturedModelBase(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext *devcon, D3DXVECTOR3&);
	~TexturedModelBase();
	void Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	void Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	void Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix);
};

