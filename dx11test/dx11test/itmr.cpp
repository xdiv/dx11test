#include "itmr.h"
#include "MD5ModelBinary.h"

itmr::itmr()
{
	pVBuffer = 0; //vertex buffer
	pIBuffer = 0; //index buffer
	texture  = 0;
	tsinst   = 0;

	instances	= 0;
	pInsBuffer	= 0;
	instanceCount = 0;

	maxInstanceCount = 256;
}


itmr::~itmr()
{
	Relese();
}

void itmr::Relese()
{	
	SAFE_RELEASE(pVBuffer); //vertex buffer
	SAFE_RELEASE(pIBuffer); //index buffer
	SAFE_DELETE(instances);
	SAFE_RELEASE(texture);
	SAFE_RELEASE(pInsBuffer);
}

void itmr::Init(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext * devcon)
{
	tsinst = InstanedShader::GetInstance(dev, hWnd, devcon);

	MD5ModelBinary * mod = new MD5ModelBinary("test.txt");
	meshv1 * mesh = NULL;
	int* list = NULL;

	mod->LoadModel(0);
//	mod->PrepareMesh(mesh, list, 0);

	vert_count = mod->GetMeshSize();
	indexCount = mod->GetIntSize();

	instances = new InstanceType_A[maxInstanceCount];

	pVBuffer	= CreateVertexBufferHelp(dev, (sizeof(meshv1) * vert_count), mesh);
	pIBuffer	= CreateIndexBufferHelp(dev, sizeof(unsigned long) * indexCount, list);
	pInsBuffer  = CreateInstanceBufferHelp(dev, sizeof(InstanceType_A) * maxInstanceCount, list);

	SAFE_DELETE(mesh);
	SAFE_DELETE(list);
	SAFE_DELETE(mod);

	D3DX11CreateShaderResourceViewFromFile(dev, L"bob_body.dds", NULL, NULL, &texture, NULL);
}

void itmr::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix)
{
	if (instanceCount == 0)
		return;

	UINT stride[2] = { sizeof(meshv1), sizeof(InstanceType_A) };
	UINT offset[2] = { 0, 0 };

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	devcon->Map(pInsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // lock the instance buffer        
	memcpy((InstanceType_A*)mappedResource.pData, instances, sizeof(InstanceType_A) * instanceCount); //overwrite instance buffer with new data
	devcon->Unmap(pInsBuffer, 0);

	ID3D11Buffer* bufferPointers[] = { pVBuffer, pInsBuffer };

	devcon->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	devcon->PSSetShaderResources(0, 1, &texture);
	tsinst->Render(devcon, worldMatrix, indexCount, instanceCount);

	instanceCount = 0;
}

void itmr::AddInstance(InstanceType_A& a)
{
	if (maxInstanceCount > instanceCount)
	{
		instances[instanceCount] = a;
		instanceCount++;
	}
		
}