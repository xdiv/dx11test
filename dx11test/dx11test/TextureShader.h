#pragma once
#include "shaderbase.h"

struct TEXVERTEX
{
	//TEXVERTEX(float x, float y, float z, float a, float b) : pos(x, y, z), texture(a, b){}
	D3DXVECTOR3 pos;      // position
	D3DXVECTOR2 texture;
};

class TextureShader : protected ShaderBase
{
private:
	static TextureShader* _instance;
	ID3D11SamplerState* pSampleState;

	UINT vert_count;
	UINT indexCount;

	//functions
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
public:
	TextureShader();
	~TextureShader();
	void Init(ID3D11Device*, HWND, ID3D11DeviceContext*);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* devcon, int indexCount, D3DXMATRIX worldMatrix, ID3D11ShaderResourceView* texture);
	ID3D11ShaderResourceView* GetTexture();
	static TextureShader* GetInstance(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	static void ShutDown();
};

