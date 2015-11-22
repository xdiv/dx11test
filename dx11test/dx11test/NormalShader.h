#pragma once

#include "DataModelBase.h"
#include "ShaderBase.h"
#include "../cdxml/math.h"
#include "ResourcesManager.h"

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

	void Init(ID3D11Device2* dev, HWND hWnd, ID3D11DeviceContext2 * devcon);
	void Render(ID3D11DeviceContext2* deviceContext, DMBdata* data);
};

