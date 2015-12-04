#include "pch.h"
#include "DirectX11.h"

#include <wrl.h>
#include <wrl/client.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;
using namespace Windows::Foundation;
using namespace std;
//using namespace Windows::Graphics::Display;
//using namespace Windows::UI::Core;
//using namespace Windows::UI::Xaml::Controls;
//using namespace Platform;

#define IFFAILED(hr) { if ( FAILED(hr) ) { status = hr; ShowError(hwnd);} }
#define IFFAILED2(hr, ln) { if ( FAILED(hr) ) { status = hr; wstring str(HresultToString(status)); str.append(ln);  MessageBox(nullptr, str.c_str(), L"Critical Error", MB_OK);} }

DirectX11::DirectX11()
	: m_d3dRenderTargetView(), m_featureLevel(D3D_FEATURE_LEVEL_9_1), m_hWnd()
{
	depthStencilBuffer = 0;
	m_depthDisabledStencilState = 0;
	m_depthStencilState = 0;
	rasterState = 0;

	alphaDisableBlendingState = 0;
	alphaEnableBlendingState = 0;
}


DirectX11::~DirectX11()
{
	
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, nullptr);
	//SAFE_RELEASE(m_swapChain);

	SAFE_RELEASE(alphaDisableBlendingState);
	SAFE_RELEASE(alphaEnableBlendingState);

	//wstring str(HresultToString(status)); str.append(L"58");

	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(m_depthStencilState);
	SAFE_RELEASE(m_depthDisabledStencilState);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(m_d3dRenderTargetView);
}

void DirectX11::ShutDown()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dDevCon->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_d3dRenderTargetView = nullptr;
	//m_d2dContext->SetTarget(nullptr);
	//m_d2dTargetBitmap = nullptr;
	m_d3dDepthStencilView = nullptr;
	m_d3dDevCon->Flush();
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
}

void DirectX11::InitD2D(HWND hwnd)
{
	// Initialize Direct2D resources.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	//options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	IFFAILED2(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &options, &m_d2dFactory), L"61");
	IFFAILED2(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), &m_dwriteFactory),L"62");
	IFFAILED(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory)));

	ComPtr<IDXGIDevice3> dxgiDevice;
	IFFAILED(m_d3dDev.As(&dxgiDevice));

	IFFAILED2(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice), L" 144");
	IFFAILED2(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext), L" 145");

	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.dpiX = 96.f;
	bitmapProperties.dpiY = 96.f;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.colorContext = nullptr;

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	IFFAILED2(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),  L" 81");
	IFFAILED2(m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(),
		&bitmapProperties,
		&m_d2dTargetBitmap), L" 83");

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
	m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

void DirectX11::InitD3D(HWND hwnd, LONG screenW, LONG screenH)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc, depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	D3D11_BLEND_DESC blendStateDescription;

#pragma region Device Resources 
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	DXGI_SWAP_CHAIN_DESC scd;
	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;
	//scd.BufferCount = 2;                                    // one back buffer
	scd.BufferDesc.Width = screenW;
	scd.BufferDesc.Height = screenH;
	//scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit m_color 
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hwnd;                                // the window to be used
	scd.SampleDesc.Count = 1;								// how many multisamples
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // Set the scan line ordering and scaling to unspecified.
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// Discard the back buffer contents after presenting.
	//scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;		// All Windows Store apps must use this SwapEffect.
	scd.Flags = 0;     // allow full-screen switching

	ComPtr<IDXGISwapChain> swpChain;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	IFFAILED2(D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&scd,
		&swpChain,
		&device,
		&m_featureLevel,
		&context), L"136");

		
	IFFAILED2(swpChain.As(&m_swapChain), L" 139");
	IFFAILED2(device.As(&m_d3dDev), L" 140");
	IFFAILED2(context.As(&m_d3dDevCon), L" 141");

	ComPtr<IDXGIDevice3> dxgiDevice;
	IFFAILED2(m_d3dDev.As(&dxgiDevice), L" 143");
#pragma endregion

	ComPtr<IDXGIAdapter> dxgiAdapter;
	IFFAILED(dxgiDevice->GetAdapter(&dxgiAdapter));

	ComPtr<IDXGIFactory2> dxgiFactory;
	IFFAILED(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));
	IFFAILED(dxgiDevice->SetMaximumFrameLatency(1));

	// get the address of the back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	IFFAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	// use the back buffer address to create the render target
	IFFAILED(m_d3dDev->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_d3dRenderTargetView));

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = screenW;
	depthBufferDesc.Height = screenH;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	IFFAILED(m_d3dDev->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer));

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	m_d3dDev->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	status = m_d3dDev->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);

	// Set the depth stencil state.
	m_d3dDevCon->OMSetDepthStencilState(m_depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	m_d3dDev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &m_d3dDepthStencilView);

	// set the render target as the back buffer
	m_d3dDevCon->OMSetRenderTargets(1, &m_d3dRenderTargetView, m_d3dDepthStencilView.Get());
	//devcon->OMSetRenderTargets(1, &m_d3dRenderTargetView, nullptr);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = true;
	//rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	m_d3dDev->CreateRasterizerState(&rasterDesc, &rasterState);
	m_d3dDevCon->RSSetState(rasterState);

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	status = m_d3dDev->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState);

	// Set the viewport
	ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));

	m_screenViewport.TopLeftX = 0;
	m_screenViewport.TopLeftY = 0;
	m_screenViewport.Width = screenW;
	m_screenViewport.Height = screenH;
	m_screenViewport.MinDepth = D3D11_MIN_DEPTH;
	m_screenViewport.MaxDepth = D3D11_MAX_DEPTH;

	m_d3dDevCon->RSSetViewports(1, &m_screenViewport);
}

void DirectX11::TurnOnAlphaBlending()
{
	float blendFactor[4];

	//ZeroMemory(blendFactor, ARRAYSIZE(blendFactor));
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_d3dDevCon->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void DirectX11::TurnOffAlphaBlending()
{
	float blendFactor[4];

	//ZeroMemory(blendFactor, ARRAYSIZE(blendFactor));
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_d3dDevCon->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void DirectX11::BeginScene()
{
	// Reset the viewport to target the whole screen.
	//m_d3dDevCon->RSSetViewports(1, &m_screenViewport);

	//ID3D11RenderTargetView *const targets[1] = { m_d3dRenderTargetView };
	//m_d3dDevCon->OMSetRenderTargets(1, targets, m_d3dDepthStencilView.Get());

	m_d3dDevCon->ClearRenderTargetView(m_d3dRenderTargetView, DirectX::Colors::Black);
	m_d3dDevCon->ClearDepthStencilView(m_d3dDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX11::EndScene()
{
	if (vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be removed.
		//m_d3dDevCon->DiscardView(m_d3dRenderTargetView);
		// Discard the contents of the depth stencil.
		//m_d3dDevCon->DiscardView(m_d3dDepthStencilView.Get());
	}
	else
		// Present as fast as possible.
		m_swapChain->Present(0, 0);

}

inline void DirectX11::Error(HWND hwnd, HRESULT error)
{
	MessageBox(hwnd, HresultToString(status), L"Critical Error", MB_OK);
}

inline void DirectX11::ShowError(HWND hwnd)
{
	Error(hwnd, this->status);
}

void DirectX11::VSinc(bool state)
{
	vsync_enabled = state;
}

void DirectX11::Trim()
{
	ComPtr<IDXGIDevice3> dxgiDevice;
	m_d3dDev.As(&dxgiDevice);

	dxgiDevice->Trim();
}

void DirectX11::TurnZBufferOn()
{
	m_d3dDevCon->OMSetDepthStencilState(m_depthStencilState, 1);
}

void DirectX11::TurnZBufferOff()
{
	m_d3dDevCon->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}