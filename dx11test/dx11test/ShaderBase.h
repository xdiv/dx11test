#pragma once

#ifndef __SHADER_BASE__
#define __SHADER_BASE__
#include "DX_Global.h"
#include "../cdxml/complex_types.h"

#include <fstream>

using namespace std;
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

struct MatrixBufferTypeA
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct PSConstBuffer
{
	float3 color;
	float transperency;
	float hasTexture;
	float hasColor;
	float2 buffer;
};

struct MatrixBufferTypeB
{
	XMMATRIX world;
};

class ShaderBase
{
protected:
	ID3D11Buffer* IVsBuffer;
	ID3D11Buffer* IPxBuffer;
	ID3D11VertexShader *pVS;    // the vertex shader
	ID3D11PixelShader *pPS;     // the pixel shader
	ID3D11InputLayout *pLayout;    // global
	ID3D11SamplerState* pSampleState;

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

protected:
	void RenderShader(ID3D11DeviceContext2* deviceContext, int indexCount);
	//void SetShaderParameters(ID3D11DeviceContext2* deviceContext, D3DXMATRIX worldMatrix);
	void Init(ID3D11Device2*, HWND, WCHAR*, WCHAR*, ID3D11DeviceContext2*, D3D11_INPUT_ELEMENT_DESC *, int);

	void CreatePixelShaderBuffer(ID3D11Device2* dev);
	//void SetShaderParameters(ID3D11DeviceContext2* devcon, PSConstBuffer buffer);
	void CreateSampler(ID3D11Device2* dev);
public:
	ShaderBase();
	~ShaderBase();
	void Release();
	//render indexd, instanced textured;
	void RenderIIT(ID3D11DeviceContext2* devcon, XMMATRIX &worldMatrix, UINT indexCount, UINT instanceCount);
	void SetVertexShaderBuffers(ID3D11DeviceContext2* devcon, XMMATRIX * data);
	void SetPixelShaderBuffers(ID3D11DeviceContext2* devcon, PSConstBuffer * data);
};

ID3D11Buffer* CreateD3D11Buffer(ID3D11Device2* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags, void * data, UINT sysMemPitch = 0, UINT sysMemSlicePitch = 0);
ID3D11Buffer* CreateD3D11BufferEmpty(ID3D11Device2* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags);
ID3D11ShaderResourceView* CreateD3D11TextureResourceView(ID3D11Device2* dev, D3D11_USAGE usage, UINT width, UINT height, D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuAccesFlags, void * data, UINT sysMemPitch, UINT sysMemSlicePitch);

inline ID3D11Buffer* CreateVertexBufferHelp		(ID3D11Device2* dev, UINT byteWidth, void * data)	{ return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data); }
inline ID3D11Buffer* CreateInstanceBufferHelp	(ID3D11Device2* dev, UINT byteWidth, void * data)	{ return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data); }
inline ID3D11Buffer* CreateIndexBufferHelp		(ID3D11Device2* dev, UINT byteWidth, void * data)    { return CreateD3D11Buffer(dev, D3D11_USAGE_DEFAULT, byteWidth, D3D11_BIND_INDEX_BUFFER	, 0						, data); }
#endif