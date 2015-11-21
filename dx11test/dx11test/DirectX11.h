#pragma once

#include "DX_Global.h"

class DirectX11
{
protected:
	IDXGISwapChain *swapchain;
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	ID3D11RenderTargetView *backbuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthDisabledStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	ID3D11BlendState *alphaEnableBlendingState, *alphaDisableBlendingState;

	bool vsync_enabled;
	HRESULT status;
public:
	DirectX11();
	~DirectX11();

	void InitD3D(HWND hwnd, LONG screenW, LONG screenH);
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();
	void VSinc(bool state);
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

protected:
	inline void Error(HWND hwnd, HRESULT error);
	inline void ShowError(HWND hwnd);
};

