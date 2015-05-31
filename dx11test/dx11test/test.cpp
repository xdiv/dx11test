#include "test.h"


test::test(ID3D11Device* dev, HWND hWnd, ID3D11DeviceContext *devcon, D3DXVECTOR3 & poz )
{
	cs = ColorShader::GetInstance(dev, hWnd, devcon);
	VERTEX OurVertices[] =
	{
		{ D3DXVECTOR3{ -0.45f, 0.5f, 0.0f }+poz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3{ 0.0f, -0.0f, 0.0f }+poz, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3{ -0.45f, -0.5f, 0.0f }+poz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	unsigned long indexCount = 3;
	unsigned long * indices = new unsigned long[3];

	indices[0] = 1;
	indices[1] = 2;
	indices[2] = 3;

	// create the vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	//D3D11_MAPPED_SUBRESOURCE vertexData, indexData;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	vertexBufferDesc.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = OurVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	dev->CreateBuffer(&vertexBufferDesc, &vertexData, &pVBuffer);       // create the buffer

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	dev->CreateBuffer(&indexBufferDesc, &indexData, &pIBuffer);

	//devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &vertexData);    // map the buffer
	//memcpy(vertexData.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	//devcon->Unmap(pVBuffer, NULL);
}


test::~test()
{
	delete cs;
}

void test::Render(ID3D11DeviceContext *devcon, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cs->RenderShader(devcon, 3, worldMatrix, viewMatrix, projectionMatrix);
}
