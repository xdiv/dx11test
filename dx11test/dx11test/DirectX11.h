#pragma once

#include "DX_Global.h"

#if defined(DEBUGW8)
#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx11.lib")
//#pragma comment (lib, "d3dx10.lib")
#endif

#if defined(DEBUGW10)
#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3d10_1.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#endif

class DirectX11
{
protected:
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device2>			m_d3dDev;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	m_d3dDevCon;
	ID3D11RenderTargetView*		m_d3dRenderTargetView;
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilState*	depthStencilState;
	ID3D11DepthStencilState*	depthDisabledStencilState;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11RasterizerState*		rasterState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	D3D_FEATURE_LEVEL			m_featureLevel;

	// Direct2D drawing components.
	Microsoft::WRL::ComPtr<ID2D1Factory2>		m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device1>		m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1>	m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

	// DirectWrite drawing components.
	Microsoft::WRL::ComPtr<IDWriteFactory2>		m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;

	bool vsync_enabled;
	HRESULT status;
public:
	DirectX11();
	~DirectX11();

	void InitD3D(HWND hwnd, LONG screenW, LONG screenH);
	void InitD2D(HWND hwnd);
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();
	void VSinc(bool state);
	void Trim();

	//d3d
	ID3D11Device2*			GetD3DDevice()			{ return m_d3dDev.Get(); }
	ID3D11DeviceContext2*	GetD3DDeviceContext()	{ return m_d3dDevCon.Get(); }

	//d2d
	ID2D1Device1*			GetD2DDevice()			{ return m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	DetD2DDeviceContext()	{ return m_d2dContext.Get(); }
	ID2D1Factory2*			GetD2DFactory()			{ return m_d2dFactory.Get(); }
	IDWriteFactory2*		GetDWriteFactory()		{ return m_dwriteFactory.Get(); }
protected:
	inline void Error(HWND hwnd, HRESULT error);
	inline void ShowError(HWND hwnd);
};

