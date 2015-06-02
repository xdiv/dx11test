#pragma once

#include "ShaderBase.h"
#include "complex_types.h"

class InstanedShader: protected ShaderBase
{
private:
	ID3D11SamplerState* pSampleState;

	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer
	ID3D11Buffer* pInsBuffer;
	ID3D11ShaderResourceView* texture;

	UINT indexCount;
	UINT instanceCount;
	UINT meshSize;
public:
	InstanedShader();
	~InstanedShader();
	void Init(ID3D11Device* dev, ID3D11DeviceContext* devcon, HWND hwnd);
	void Prepare(ID3D11Device* dev);
	void Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix);
};

