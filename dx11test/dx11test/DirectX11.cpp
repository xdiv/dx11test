#include "DirectX11.h"

#include <wrl.h>
#include <wrl/client.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;
using namespace Windows::Foundation;
//using namespace Windows::Graphics::Display;
//using namespace Windows::UI::Core;
//using namespace Windows::UI::Xaml::Controls;
//using namespace Platform;

#define IFFAILED(hr) { if ( FAILED(hr) ) { status = hr; ShowError(hwnd);} }

DirectX11::DirectX11()
{
	m_featureLevel = D3D_FEATURE_LEVEL_9_1;
	//m_swapChain = 0;
	m_d3dRenderTargetView = 0;
	depthStencilBuffer = 0;
	depthDisabledStencilState = 0;
	depthStencilState = 0;
	depthStencilView = 0;
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

	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthDisabledStencilState);
	SAFE_RELEASE(depthStencilBuffer);

	SAFE_RELEASE(m_d3dRenderTargetView);
	/*SAFE_RELEASE(m_d3dDevCon);
	SAFE_RELEASE(m_d3dDev);*/
}

void DirectX11::InitD2D(HWND hwnd)
{
	// Initialize Direct2D resources.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	IFFAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &options, &m_d2dFactory));
	IFFAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), &m_dwriteFactory));
	IFFAILED(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory)));
}

void DirectX11::InitD3D(HWND hwnd, LONG screenW, LONG screenH)
{
	DXGI_SWAP_CHAIN_DESC scd;
	D3D11_VIEWPORT viewport;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc, depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

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

	D3D11_BLEND_DESC blendStateDescription;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swpChain;
	ComPtr<IDXGIDevice3> dxgiDevice;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIFactory2> dxgiFactory;
	ComPtr<IDXGISurface2> dxgiBackBuffer;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif

	/*
	IDXGIFactory* factory;
	HRESULT result;
	IDXGIOutput* adapterOutput;

	IDXGIAdapter* adapter;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	// create a struct to hold information about the swap chain


	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	// Use the factory to create an adapter for the primary graphics interface (video card)
	factory->EnumAdapters(0, &adapter);

	// Enumerate the primary adapter output (monitor).
	adapter->EnumOutputs(0, &adapterOutput);

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);

	displayModeList = new DXGI_MODE_DESC[numModes];

	// Now fill the display mode list structures.
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	for (i = 0; i<numModes; i++)
	{
	if (displayModeList[i].Width == (unsigned int)screenWidth)
	{
	if (displayModeList[i].Height == (unsigned int)screenHeight)
	{
	numerator = displayModeList[i].RefreshRate.Numerator;
	denominator = displayModeList[i].RefreshRate.Denominator;
	}
	}
	}

	adapter->GetDesc(&adapterDesc);

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;
	*/

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Width = screenW;
	scd.BufferDesc.Height = screenH;
	//scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color 
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

	IFFAILED(D3D11CreateDeviceAndSwapChain(nullptr,
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
		&context));

	IFFAILED(device.As(&m_d3dDev));
	IFFAILED(context.As(&m_d3dDevCon));
	IFFAILED(m_d3dDev.As(&dxgiDevice));
	IFFAILED(dxgiDevice->GetAdapter(&dxgiAdapter));
	IFFAILED(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));
	IFFAILED(swpChain.As(&m_swapChain));

	IFFAILED(dxgiDevice->SetMaximumFrameLatency(1));

	IFFAILED(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice));
	IFFAILED(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	IFFAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

	// use the back buffer address to create the render target
	IFFAILED(m_d3dDev->CreateRenderTargetView(pBackBuffer, nullptr, &m_d3dRenderTargetView));

	//pBackBuffer->Release();
	//pBackBuffer = 0;

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
	m_d3dDev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

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
	status = m_d3dDev->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);

	// Set the depth stencil state.
	m_d3dDevCon->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	m_d3dDev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

	// set the render target as the back buffer
	m_d3dDevCon->OMSetRenderTargets(1, &m_d3dRenderTargetView, depthStencilView);
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
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenW;
	viewport.Height = screenH;
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	m_d3dDevCon->RSSetViewports(1, &viewport);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.dpiX = 42.0f;
	bitmapProperties.dpiY = 42.0f;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.colorContext = nullptr;

	status = (m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));
	status = m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(), 
		&bitmapProperties, 
		&m_d2dTargetBitmap);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
	m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
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
	const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_d3dDevCon->ClearRenderTargetView(m_d3dRenderTargetView, color);
	m_d3dDevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
		//m_d3dDevCon->DiscardView(depthStencilView);
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
	m_d3dDevCon->OMSetDepthStencilState(depthStencilState, 1);
}

void DirectX11::TurnZBufferOff()
{
	m_d3dDevCon->OMSetDepthStencilState(depthDisabledStencilState, 1);
}