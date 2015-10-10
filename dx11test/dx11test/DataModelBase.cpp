#include "DataModelBase.h"
#include "../gml/MD5ModelBinary.h"

DMBdata::DMBdata()
{
	pVBuffer = 0; //vertex buffer
	pIBuffer = 0; //index buffer
	texture = 0;

	pInsBuffer = 0;
	instanceCount = 0;
	maxInstanceCount = 256;
	instances = 0;
	stride[0] = 0;
	stride[1] = 0;

	offset[0] = 0;
	offset[1] = 0;
}

DMBdata::~DMBdata()
{
	SAFE_RELEASE(pVBuffer); //vertex buffer
	SAFE_RELEASE(pIBuffer); //index buffer
	SAFE_RELEASE(texture);
	SAFE_RELEASE(pInsBuffer);
	SAFE_DELETE(instances);
}

DataModelBase::DataModelBase()
{
	data = 0;
	data = new DMBdata();

	//id = IdGenerator++;
}


DataModelBase::~DataModelBase()
{
	SAFE_DELETE(data);
}

void DataModelBase::AddInstance(InstanceType_B a)
{
	if (data->maxInstanceCount > data->instanceCount)
	{
		data->instances[data->instanceCount] = a;
		data->instanceCount++;
	}
}

DMBdata* DataModelBase::GetData()
{
	return data;
}

void DataModelBase::UpdateInstanceBuffer(ID3D11DeviceContext * devcon)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	devcon->Map(data->pInsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // lock the instance buffer        
	memcpy((InstanceType_B*)mappedResource.pData, data->instances, sizeof(InstanceType_B) * data->maxInstanceCount); //overwrite instance buffer with new data
	devcon->Unmap(data->pInsBuffer, 0);
}

void DataModelBase::LoadTestModel1(ID3D11Device * dev)
{
	mesh2d mesh[4] =
	{
		mesh2d(0, 0, 0, 0, 0),
		mesh2d(0, 1, 0, 0, 1),
		mesh2d(1, 1, 0, 1, 1),
		mesh2d(1, 0, 0, 1, 0)
	};

	UINT index[] = {0, 1, 2, 2, 3, 0 };
	//UINT index[] = { 0, 3, 1, 3, 2, 1 };

	//UINT index[] = { 0, 1, 3, 2, 1, 3 };
	data->vert_count = sizeof(mesh) / sizeof(mesh2d);
	data->indexCount = sizeof(index) / sizeof(UINT);

	data->instances = new InstanceType_B[data->maxInstanceCount];

	data->stride[0] = sizeof(mesh2d);
	data->stride[1] = sizeof(InstanceType_B);

	data->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * data->vert_count), mesh);
	data->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * data->indexCount, index);
	data->pInsBuffer = CreateInstanceBufferHelp(dev, sizeof(InstanceType_B) * data->maxInstanceCount, data->instances);
}

void DataModelBase::LoadTestModel2(ID3D11Device * dev)
{
	MD5ModelBinary * mod = new MD5ModelBinary("test.txt");
	mesh2d * mesh = NULL;
	UINT * list = NULL;

	mod->LoadModel(0);
	mod->PrepareMesh(mesh, list, 0);

	data->vert_count = mod->GetMeshSize();
	data->indexCount = mod->GetIntSize();

	data->instances = new InstanceType_B[data->maxInstanceCount];

	data->stride[0] = sizeof(mesh2d);
	data->stride[1] = sizeof(InstanceType_B);

	data->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * data->vert_count), mesh);
	data->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * data->indexCount, list);
	data->pInsBuffer = CreateInstanceBufferHelp(dev, sizeof(InstanceType_B) * data->maxInstanceCount, data->instances);

	SAFE_DELETE(mesh);
	SAFE_DELETE(list);
	SAFE_DELETE(mod);

	//D3DCreateShaderResourceViewFromFile(dev, L"bob_body.dds", NULL, NULL, &(data->texture), NULL);
	//D3DCreateShaderResourceViewFromFile(dev, L"bob_body.dds", NULL, NULL, &(data->texture), NULL);
	DirectX::C
}

void DataModelBase::LoadTestModel3(ID3D11Device * dev)
{
	mesh2d mesh[4] =
	{
		mesh2d(-0.5, -0.5, 0, 0, 0),
		mesh2d(-0.5, 0.5, 0, 0, 1),
		mesh2d(0.5, 0.5, 0, 1, 1),
		mesh2d(0.5, -0.5, 0, 1, 0)
	};

	UINT index[] = { 0, 1, 2, 2, 3, 0 };
	data->vert_count = sizeof(mesh) / sizeof(mesh2d);
	data->indexCount = sizeof(index) / sizeof(UINT);

	data->instances = new InstanceType_B[data->maxInstanceCount];

	data->stride[0] = sizeof(mesh2d);
	data->stride[1] = sizeof(InstanceType_B);

	data->pVBuffer = CreateVertexBufferHelp(dev, (sizeof(mesh2d) * data->vert_count), mesh);
	data->pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * data->indexCount, index);
	data->pInsBuffer = CreateInstanceBufferHelp(dev, sizeof(InstanceType_B) * data->maxInstanceCount, data->instances);
}
