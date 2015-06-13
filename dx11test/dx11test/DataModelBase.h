#pragma once

#include "DX_Global.h"
#include "complex_types.h"
#include "InstanedShader.h"
UINT IdGenerator = 0;

class DMBdata
{
public:
	ID3D11Buffer* pVBuffer; //vertex buffer
	ID3D11Buffer* pIBuffer; //index buffer
	ID3D11Buffer* pInsBuffer; //
	ID3D11ShaderResourceView* texture; //reikes daugiau teksturu

	UINT vert_count;
	UINT indexCount;
	UINT maxInstanceCount;
	UINT instanceCount;

	InstanceType_A* instances;
	DMBdata();
	~DMBdata();
};

class DataModelBase
{
private:
	UINT id;
	UINT type;
	UINT shader;

protected:
	DMBdata* data;

	DataModelBase(DataModelBase&) {};
public:
	DataModelBase();
	~DataModelBase();

	void Init(DMBdata data);
	void AddInstance(InstanceType_A &);
	void UpdateInstanceBuffer();
};

