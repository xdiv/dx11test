#pragma once

#ifndef __SHADER_BASE__
#define __SHADER_BASE__
#include "DX_Global.h"
#include "complex_types.h"

#include <fstream>
#include "PublicData.h"

using namespace std;


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

struct MatrixBufferTypeA
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct PSConstBuffer
{
	float hasTexture;
	float hasColor;
	float4 color;
	float transperency;
};

struct MatrixBufferTypeB
{
	D3DXMATRIX world;
};

class ShaderBase
{
protected:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* IPxBuffer;
	ID3D11VertexShader *pVS;    // the vertex shader
	ID3D11PixelShader *pPS;     // the pixel shader
	ID3D11InputLayout *pLayout;    // global

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

protected:
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix);
	void Init(ID3D11Device*, HWND, WCHAR*, WCHAR*, ID3D11DeviceContext*, D3D11_INPUT_ELEMENT_DESC *, int);
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void CreatePixelShaderBuffer(ID3D11Device* dev);
	void SetShaderParameters(ID3D11DeviceContext* devcon, PSConstBuffer buffer);
public:
	ShaderBase();
	~ShaderBase();
	void Relese();
};

ID3D11Buffer* CreateD3D11Buffer(ID3D11Device* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags, void * data);
ID3D11Buffer* CreateD3D11BufferEmpty(ID3D11Device* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags);

inline ID3D11Buffer* CreateVertexBufferHelp		(ID3D11Device* dev, UINT byteWidth, void * data)	{ return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data); }
inline ID3D11Buffer* CreateInstanceBufferHelp	(ID3D11Device* dev, UINT byteWidth, void * data)	{ return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data); }
inline ID3D11Buffer* CreateIndexBufferHelp		(ID3D11Device* dev, UINT byteWidth, void * data)    { return CreateD3D11Buffer(dev, D3D11_USAGE_DEFAULT, byteWidth, D3D11_BIND_INDEX_BUFFER	, 0						, data); }
#endif