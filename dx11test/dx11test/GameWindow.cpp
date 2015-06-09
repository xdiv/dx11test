#include "GameWindow.h"


GameWindow::GameWindow(int& w, int& h, LPCWSTR t, float screenNear, float screenDepth)
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
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;

	this->screenNear = screenNear;
	this->screenDepth = screenDepth;
}


GameWindow::~GameWindow()
{
	ShutDown();
	DestroyWindow(hWnd);
	hWnd = NULL;

	UnregisterClass(L"WindowClass1", hInstance);
	hInstance = NULL;
}

void GameWindow::ShutDown()
{
	
	swapchain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(rasterState);

	depthStencilView->Release();
	depthStencilView = 0;

	depthStencilState->Release();
	depthStencilState = 0;

	depthStencilBuffer->Release();
	depthStencilBuffer = 0;

	backbuffer->Release();
	backbuffer = 0;

	devcon->Release();
	devcon = 0;

	dev->Release();
	dev = 0;

	swapchain->Release();
	swapchain = 0;
}

void GameWindow::InitializeWindows()
{
	WNDCLASSEX wc;
	hInstance = GetModuleHandle(NULL);

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

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
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;
	D3D_FEATURE_LEVEL featureLevel;
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
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

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
	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		//D3D11_CREATE_DEVICE_DEBUG,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	
	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
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

	dev->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	
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
	// Set the depth stencil state.
	devcon->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	
	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
	//devcon->OMSetRenderTargets(1, &backbuffer, NULL);

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

	// Set the viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &viewport);


	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

void GameWindow::Run()
{
	MSG msg;
	
	GameInit();
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			break;
		}else
		{
			devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			Update();
			Render();
			RenderInterface();
			
			swapchain->Present(1, 0);
		}
		
	}

	GameShutDown();
}

void GameWindow::Close()
{
	swapchain->Release();

	dev->Release();
	devcon->Release();
	backbuffer->Release();
}


void GameWindow::GameInit()
{
	ShowCursor(FALSE);
	camera = new Camera();
	input = new ButtonsActionMap(hInstance, hWnd);

	camera->SetPosition(0.0f, -50.0f, 3.0f);
	input->SetCamera(camera);

	insTest = new itmr();
	insTest->Init(dev, hWnd, devcon);
}
void GameWindow::GameShutDown()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(insTest);
}

void GameWindow::Update()
{
	camera->Render();
	input->Update();
	camera->GetViewMatrix(viewMatrix);
	finalMatrix = worldMatrix * viewMatrix * projectionMatrix;

	insTest->AddInstance(InstanceType_A(0, 0, 0));
	insTest->AddInstance(InstanceType_A(15, 0, 0));
	insTest->AddInstance(InstanceType_A(-15, 0, 0));
}
void GameWindow::Render()
{
	insTest->Render(devcon, finalMatrix);
}
void GameWindow::RenderInterface()
{

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
				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;
			}
		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
