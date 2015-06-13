#include "DataModelBase.h"

DMBdata::DMBdata()
{
	pVBuffer = 0; //vertex buffer
	pIBuffer = 0; //index buffer
	texture = 0;

	pInsBuffer = 0;
	instanceCount = 0;
	maxInstanceCount = 256;
	instances = new InstanceType_A[maxInstanceCount];
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

	id = IdGenerator++;
}


DataModelBase::~DataModelBase()
{
	SAFE_DELETE(data);
}

void DataModelBase::AddInstance(InstanceType_A& a)
{
	if (data->maxInstanceCount > data->instanceCount)
	{
		data->instances[data->instanceCount] = a;
		data->instanceCount++;
	}

}
