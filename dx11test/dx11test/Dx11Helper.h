#pragma once

#ifndef __DX11HELPER__
#define __DX11HELPER__
#include "pch.h"

namespace Dx11Helper
{
	inline ID3D11Buffer* CreateD3D11Buffer(ID3D11Device2* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags, void * data, UINT sysMemPitch, UINT sysMemSlicePitch)
	{
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA subData;
		ID3D11Buffer * buffer;
		buffer = 0;

		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

		bufferDesc.Usage = usage;                // write access access by CPU and GPU
		bufferDesc.ByteWidth = byteWidth;						   // size sizeof(struct)*elementCount
		bufferDesc.BindFlags = bindFlags;       // use as a vertex buffer
		bufferDesc.CPUAccessFlags = cpuAccesFlags;    // allow CPU to write in buffer
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		//vertexData.pSysMem = OurVertices;
		subData.pSysMem = data;
		subData.SysMemPitch = sysMemPitch;
		subData.SysMemSlicePitch = sysMemSlicePitch;

		//delete mesh;

		dev->CreateBuffer(&bufferDesc, &subData, &buffer);       // create the buffer
		return buffer;
	}

	inline ID3D11ShaderResourceView* CreateD3D11TextureResourceView(ID3D11Device2* dev, D3D11_USAGE usage, UINT width, UINT height, D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuAccesFlags, void * data, UINT sysMemPitch, UINT sysMemSlicePitch)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc; //textureDesc
		ZeroMemory(&desc, sizeof(desc));
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc; //srvDesc
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		ID3D11Texture2D* tex = nullptr; //m_texture
		ID3D11ShaderResourceView *textureView = nullptr; //m_textureView
		const UINT TexturePixelSize = 4;


		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = usage;
		desc.BindFlags = bindFlags;
		desc.CPUAccessFlags = cpuAccesFlags;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = data;
		initData.SysMemPitch = sizeof(UINT8) * sysMemPitch * 4;
		/*initData.SysMemPitch = sysMemPitch * 4;
		initData.SysMemSlicePitch = sysMemSlicePitch * 4;*/

		hr = dev->CreateTexture2D(&desc, &initData, &tex);

		//hr = dev->CreateTexture2D(&desc, nullptr, &tex);

		if (SUCCEEDED(hr) && tex != 0)
		{
			//if (textureView != 0)
			{

				SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				SRVDesc.Texture2D.MipLevels = 1;

				hr = dev->CreateShaderResourceView(tex, &SRVDesc, &textureView);

				//tex->Release();
				//delete(tex);
				if (FAILED(hr))
				{
					return nullptr;
				}

				return textureView;
			}
		}
		else
		{
			//MessageBox()
			return nullptr;
		}
	}

	inline ID3D11Buffer* CreateD3D11BufferEmpty(ID3D11Device2* dev, D3D11_USAGE usage, UINT byteWidth, UINT bindFlags, UINT cpuAccesFlags)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ID3D11Buffer * buffer;
		buffer = 0;

		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = usage;                // write access access by CPU and GPU
		bufferDesc.ByteWidth = byteWidth;						   // size sizeof(struct)*elementCount
		bufferDesc.BindFlags = bindFlags;       // use as a vertex buffer
		bufferDesc.CPUAccessFlags = cpuAccesFlags;    // allow CPU to write in buffer
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		dev->CreateBuffer(&bufferDesc, nullptr, &buffer);       // create the buffer
		return buffer;
	}

	inline ID3D11Buffer* CreateVertexBufferHelp(ID3D11Device2* dev, UINT byteWidth, void * data) { return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data, 0, 0); }
	inline ID3D11Buffer* CreateInstanceBufferHelp(ID3D11Device2* dev, UINT byteWidth, void * data) { return CreateD3D11Buffer(dev, D3D11_USAGE_DYNAMIC, byteWidth, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, data, 0, 0); }
	inline ID3D11Buffer* CreateIndexBufferHelp(ID3D11Device2* dev, UINT byteWidth, void * data) { return CreateD3D11Buffer(dev, D3D11_USAGE_DEFAULT, byteWidth, D3D11_BIND_INDEX_BUFFER, 0, data, 0, 0); }

}

#endif