#pragma once

#include "pch.h"

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

__interface ID2D
{
	ID2D1Device1*			GetD2DDevice();
	ID2D1DeviceContext1*	GetD2DDeviceContext();
	ID2D1Factory2*			GetD2DFactory();
	IDWriteFactory2*		GetDWriteFactory();
	HWND					GetHwnd();
};

__interface ID3D
{
	ID3D11Device2*			GetD3DDevice();
	ID3D11DeviceContext2*	GetD3DDeviceContext();
	HWND					GetHwnd();
};

class DirectX11 : public ID3D, public ID2D
{
public:
	DirectX11();
	~DirectX11();
	void ShutDown();
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

	void FullScreenSwitch(bool state) { m_swapChain->SetFullscreenState(state, nullptr); };

	//d3d
	ID3D11Device2*			GetD3DDevice()			{ return m_d3dDev.Get(); }
	ID3D11DeviceContext2*	GetD3DDeviceContext()	{ return m_d3dDevCon.Get(); }
	ID3D*					GetD3D()				{ return (ID3D*)this; }
	HWND					GetHwnd()				{ return m_hWnd; }

	//d2d
	ID2D1Device1*			GetD2DDevice()			{ return m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	GetD2DDeviceContext()	{ return m_d2dContext.Get(); }
	ID2D1Factory2*			GetD2DFactory()			{ return m_d2dFactory.Get(); }
	IDWriteFactory2*		GetDWriteFactory()		{ return m_dwriteFactory.Get(); }
	ID2D*					GetD2D()				{ return (ID2D*)this; };

protected:
	inline void Error(HWND hwnd, HRESULT error);
	inline void ShowError(HWND hwnd);

	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device2>			m_d3dDev;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	m_d3dDevCon;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;
	ID3D11RenderTargetView*		m_d3dRenderTargetView;
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilState*	m_depthDisabledStencilState;
	ID3D11RasterizerState*		rasterState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	D3D_FEATURE_LEVEL			m_featureLevel;
	D3D11_VIEWPORT				m_screenViewport;

	// Direct2D drawing components.
	Microsoft::WRL::ComPtr<ID2D1Factory2>		m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device1>		m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1>	m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;
	//ID2D1Bitmap1* m_d2dTargetBitmap;

	//// DirectWrite drawing components.
	Microsoft::WRL::ComPtr<IDWriteFactory2>		m_dwriteFactory;
	//Microsoft::WRL::ComPtr<IWICImagingFactory2>  m_wicFactory // failina release ir delete
	IWICImagingFactory2*	m_wicFactory;

	bool	vsync_enabled;
	HRESULT status;
	HWND	m_hWnd;
};

