#include "GameWindow.h"
#include "PublicData.h"

GameWindow* GameWindow::sInst = 0;

GameWindow::GameWindow(LONG& w, LONG& h, LPCWSTR t, float screenNear, float screenDepth, float aspectRatio)
{
	title = t;
	screenHeight = h;
	screenWidth = w;
	hInstance = 0;

	swapchain = 0;
	dev = 0;
	devcon = 0;
	backbuffer = 0;
	depthStencilBuffer = 0;
	depthDisabledStencilState = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;

	alphaDisableBlendingState = 0;
	alphaEnableBlendingState = 0;

	this->screenNear = screenNear;
	this->screenDepth = screenDepth;
	this->aspectRatio = aspectRatio;

	//pb = (this);
}


GameWindow::~GameWindow()
{
	ShutDown();
	DestroyWindow(hWnd);
	hWnd = nullptr;

	UnregisterClass(L"WindowClass1", hInstance);
	hInstance = nullptr;
}

void GameWindow::ShutDown()
{
	if (swapchain)
		swapchain->SetFullscreenState(false, nullptr);
	SAFE_RELEASE(swapchain);

	SAFE_RELEASE(alphaDisableBlendingState);
	SAFE_RELEASE(alphaEnableBlendingState);

	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthDisabledStencilState);
	SAFE_RELEASE(depthStencilBuffer);

	SAFE_RELEASE(backbuffer);
	SAFE_RELEASE(devcon);
	SAFE_RELEASE(dev);
}

void GameWindow::InitializeWindows()
{
	WNDCLASSEX wc;
	hInstance = GetModuleHandle(nullptr);

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	//RECT wr = { 0, 0, screenWidth, screenHeight };
	//AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"WindowClass1",    // name of the window class
		title,   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		screenWidth,    // width of the window
		screenHeight,    // height of the window
		nullptr,    // we have no parent window, nullptr
		nullptr,    // we aren't using menus, nullptr
		hInstance,    // application handle
		nullptr);    // used with multiple windows, nullptr

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
}

void GameWindow::InitD3D()
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC scd;
	D3D11_VIEWPORT viewport;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc, depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_BLEND_DESC blendStateDescription;
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
	scd.BufferDesc.Width = screenWidth;
	scd.BufferDesc.Height = screenHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // Set the scan line ordering and scaling to unspecified.
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// Discard the back buffer contents after presenting.
	scd.Flags = 0;     // allow full-screen switching

	//scd.SampleDesc.Count = 1;                               // how many multisamples
	//scd.SampleDesc.Quality = 0;
	
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	// create a device, device context and swap chain using the information in the scd struct
	result = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		//D3D11_CREATE_DEVICE_DEBUG,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		nullptr,
		&devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	
	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	pBackBuffer->Release();
	pBackBuffer = 0;
	
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	dev->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
	
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
	dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

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
	result = dev->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);

	// Set the depth stencil state.
	devcon->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	
	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
	//devcon->OMSetRenderTargets(1, &backbuffer, nullptr);

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

	dev->CreateRasterizerState(&rasterDesc, &rasterState);
	devcon->RSSetState(rasterState);

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
	result = dev->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState);

	// Create the blend state using the description.
	//result = dev->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState);

	// Set the viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &viewport);

	BuildWorldMatrix();
}

void GameWindow::BuildWorldMatrix()
{
	float fieldOfView, screenAspect;
	// Setup the projection matrix.
	fieldOfView = (float)XM_PI / aspectRatio;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	world3DMatrix = XMMatrixIdentity();
	world2DMatrix = world3DMatrix;
	world2DMatrix.r[2] = DirectX::XMVectorZero();
	//world2DMatrix.r[3] = DirectX::XMVectorZero();
	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, +0.0f, 1);
}

void GameWindow::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

void GameWindow::TurnOnAlphaBlending()
{
	float blendFactor[4];

	//ZeroMemory(blendFactor, ARRAYSIZE(blendFactor));
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	devcon->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void GameWindow::TurnOffAlphaBlending()
{
	float blendFactor[4];

	//ZeroMemory(blendFactor, ARRAYSIZE(blendFactor));
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	devcon->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void GameWindow::BeginScene()
{
	const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GameWindow::EndScene()
{
	if (vsync_enabled)
		// Lock to screen refresh rate.
		swapchain->Present(1, 0);
	else
		// Present as fast as possible.
		swapchain->Present(0, 0);

}

XMMATRIX GameWindow::GetWorl3DMatrix()
{
	return world3DMatrix;
}

XMMATRIX GameWindow::GetWorl2DMatrix()
{
	return world2DMatrix;
}

XMMATRIX GameWindow::GetOrtoM()
{
	return orthoMatrix;
}

XMMATRIX GameWindow::GetProjectionM()
{
	return projectionMatrix;
}

ID3D11Device* GameWindow::GetDevice()
{
	return dev;
}

ID3D11DeviceContext* GameWindow::GetDeviceContext()
{
	return devcon;
}

HWND GameWindow::GetHwnd()
{
	return hWnd;
}

void GameWindow::VSinc(bool state)
{
	vsync_enabled = state;
}

HINSTANCE GameWindow::GetHinstance()
{
	return hInstance;
}

void GameWindow::SetWindowSize(LONG width, LONG heigth)
{
	screenWidth = width;
	screenHeight = heigth;
	BuildWorldMatrix();
}

void GameWindow::SetInstance(GameWindow * gw)
{
	if (sInst)
		sInst = 0;
	sInst = gw;
}

GameWindow * GameWindow::GetInstance()
{
	return sInst;
}

void GameWindow::TurnZBufferOn()
{
	devcon->OMSetDepthStencilState(depthStencilState, 1);
}

void GameWindow::TurnZBufferOff()
{
	devcon->OMSetDepthStencilState(depthDisabledStencilState, 1);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_SIZE:
		{
			RECT rect;
			if (GetWindowRect(hWnd, &rect))
			{
				if (GameWindow::GetInstance())
					GameWindow::GetInstance()->SetWindowSize(rect.right - rect.left, rect.bottom - rect.top);
			}
		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
