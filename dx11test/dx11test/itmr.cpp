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
	mod->PrepareMesh(mesh, list, 0);

	vert_count = mod->GetMeshSize();
	indexCount = mod->GetIntSize();

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = sizeof(meshv1) * vert_count;             // size is the TEXVERTEX struct * 3
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	data.pSysMem = mesh;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	dev->CreateBuffer(&bufferDesc, &data, &pVBuffer);       // create the buffer

	SAFE_DELETE(mesh);
	SAFE_DELETE(mod);

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	data.pSysMem = list;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	dev->CreateBuffer(&bufferDesc, &data, &pIBuffer);

	SAFE_DELETE(list);

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	instances = new InstanceType_A[maxInstanceCount];

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bufferDesc.ByteWidth = sizeof(InstanceType_A) * maxInstanceCount;             // size is the TEXVERTEX struct * 3
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//vertexData.pSysMem = OurVertices;
	data.pSysMem = instances;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	dev->CreateBuffer(&bufferDesc, &data, &pInsBuffer);

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
	memcpy((InstanceType_A*)mappedResource.pData, instances, sizeof(InstanceType_A) * instanceCount + 1); //overwrite instance buffer with new data
	devcon->Unmap(pInsBuffer, 0);

	ID3D11Buffer* bufferPointers[] = { pVBuffer, pInsBuffer };

	devcon->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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