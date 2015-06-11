#include "TexturedModelBase.h"

TexturedModelBase::TexturedModelBase(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext *devcon, D3DXVECTOR3 & poz)
{
	pVBuffer = 0;
	pIBuffer = 0;
	texture  = 0;
	tsinst   = 0;

	tsinst = TextureShader::GetInstance(dev, hWnd, devcon);

	MD5ModelBinary * mod = new MD5ModelBinary("test.txt");
	meshv1 * mesh = NULL;
	int* list = NULL;

	mod->LoadModel(0);
	mod->PrepareMesh(mesh, list, 0);

	vert_count = mod->GetMeshSize();
	indexCount = mod->GetIntSize();

	SAFE_DELETE (mod);

	pVBuffer = CreateVertexBufferHelp(dev, (sizeof(meshv1) * vert_count), mesh);
	pIBuffer = CreateIndexBufferHelp(dev, sizeof(unsigned long) * indexCount, list);

	SAFE_DELETE(mesh);
	SAFE_DELETE(list);

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
	tsinst->RenderShader(devcon, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

void TexturedModelBase::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix)
{
	UINT stride = sizeof(meshv1);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	tsinst->RenderShader(devcon, indexCount, worldMatrix, texture);
}
