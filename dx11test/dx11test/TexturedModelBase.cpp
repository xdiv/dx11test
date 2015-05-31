#include "TexturedModelBase.h"

TexturedModelBase::TexturedModelBase(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext *devcon, D3DXVECTOR3 & poz)
{
	pVBuffer = 0;
	pIBuffer = 0;
	texture  = 0;
	tsinst   = 0;

	tsinst = TextureShader::GetInstance(dev, hWnd, devcon);

	MD5Model * mod = new MD5Model();
	meshv1 * mesh = NULL;
	int* list = NULL;

	//MD5ModelBinary * modb = new MD5ModelBinary();

	//modb->WriteModelToFile("test.txt", mod->GetMeshSize(), mod->GetJointsSize(), mod->GetMesh(), mod->GetBones());

	//SAFE_DELETE(modb);

	//mod.LoadModel("untl.md5mesh");
	mod->LoadModel("bob.md5mesh");
	mod->PrepareMesh(mesh, list, 0);
	

	vert_count = mod->GetMeshSize(0);
	indexCount = mod->GetIntSize(0);

	SAFE_DELETE (mod);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	vertexBufferDesc.ByteWidth = sizeof(meshv1) * vert_count;             // size is the TEXVERTEX struct * 3
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//vertexData.pSysMem = OurVertices;
	vertexData.pSysMem = mesh;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//delete mesh;

	dev->CreateBuffer(&vertexBufferDesc, &vertexData, &pVBuffer);       // create the buffer

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = list;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	dev->CreateBuffer(&indexBufferDesc, &indexData, &pIBuffer);
	//delete list;
	//D3DX11CreateShaderResourceViewFromFile(dev, L"texttest.jpg", NULL, NULL, &texture, NULL);
	D3DX11CreateShaderResourceViewFromFile(dev, L"bob_body.dds", NULL, NULL, &texture, NULL);
}


TexturedModelBase::~TexturedModelBase()
{
	SAFE_DELETE(pVBuffer);
	SAFE_DELETE(pIBuffer);
	SAFE_DELETE(texture);
}

void TexturedModelBase::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	UINT stride = sizeof(meshv1);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tsinst->RenderShader(devcon, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

void TexturedModelBase::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix)
{
	UINT stride = sizeof(meshv1);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tsinst->RenderShader(devcon, indexCount, worldMatrix, texture);
}
