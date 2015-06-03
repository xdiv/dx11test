#pragma once
#include "DX_Global.h"
#include "complex_types.h"
#include "InstanedShader.h"

#include <vector>
using namespace std;

//instanced textured model renderer
class itmr
{
private:
	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer
	ID3D11Buffer* pInsBuffer; //
	ID3D11ShaderResourceView* texture;
	InstanedShader* tsinst;

	UINT vert_count;
	UINT indexCount;
	UINT maxInstanceCount;
	UINT instanceCount;
	
	InstanceType_A* instances;
public:
	itmr();
	~itmr();

	void Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon);
	void Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix);
	void Relese();
	void AddInstance(InstanceType_A &);
};

