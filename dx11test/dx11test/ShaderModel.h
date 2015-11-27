#pragma once
#include "pch.h"

struct MatrixBufferTypeA
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
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
	DirectX::XMMATRIX world;
};
