#pragma once

#ifndef __SHADER_BASE__
#define __SHADER_BASE__
#include "pch.h"
#include "../cdxml/complex_types.h"
#include "ShaderModel.h"
#include <fstream>
#include "DirectX11.h"

using namespace std;
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class ShaderBase
{
public:
	ShaderBase(ID3D * d3d);
	~ShaderBase();
	void Release();
	//render indexd, instanced textured;
	void SetVertexShaderBuffers(XMMATRIX * data);
	void SetPixelShaderBuffers(PSConstBuffer * data);

protected:
	ID3D11Buffer*		m_IVsBuffer;
	ID3D11Buffer*		m_IPxBuffer;
	ID3D11VertexShader*	m_VS;    // the vertex shader
	ID3D11PixelShader*	m_PS;     // the pixel shader
	ID3D11InputLayout*	m_Layout;    // global
	ID3D11SamplerState* m_SampleState;

	ID3D *				m_d3d;
	
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
	void Init(WCHAR*, WCHAR*, D3D11_INPUT_ELEMENT_DESC *, int);
	void CreatePixelShaderBuffer();
	void CreateSampler();
};
#endif