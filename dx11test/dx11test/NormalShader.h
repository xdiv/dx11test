#pragma once

#include "DataModelBase.h"
#include "ShaderBase.h"
#include "DXMath.h"

class NormalShader : public ShaderBase
{
private:
	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer

	UINT vert_count;
	UINT indexCount;
public:
	NormalShader() {};
	~NormalShader() {};

	void Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	void Render(ID3D11DeviceContext* deviceContext, DMBdata* data, ID3D11ShaderResourceView* texture);
};

