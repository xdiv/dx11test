#pragma once

#ifndef __BASE_MODELS__
#define __BASE_MODELS__

#include "DX_Global.h"
#include "..\cdxml\complex_types.h"
#include "ShaderBase.h"

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
	UINT stride[2];
	UINT offset[2];

	InstanceType_B* instances;
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

	void AddInstance(InstanceType_B );
	void UpdateInstanceBuffer(ID3D11DeviceContext * devcon);

	void LoadTestModel1(ID3D11Device * dev);
	void LoadTestModel2(ID3D11Device * dev);
	void LoadTestModel3(ID3D11Device * dev);
	void LoadTestModel4(ID3D11Device * dev);

	DMBdata* GetData();
};

#endif

