#include "pch.h"
#include "GameWindow.h"

GameWindow* GameWindow::sInst = 0;

GameWindow::GameWindow(LONG& w, LONG& h, LPCWSTR t, float screenNear, float screenDepth)
	: m_screen()
{
	title = t;
	//screenHeight = h;
	//screenWidth = w;
	//aspectRatio = (float)w / (float)h;
	hInstance = 0;

	m_screen.SetRez(w, h);
	m_screen.SetClip(screenNear, screenDepth);

	//hWnd = 0;

	//this->screenNear = screenNear;
	//this->screenDepth = screenDepth;
	//this->aspectRatio = aspectRatio;

	//pb = (this);
}


GameWindow::~GameWindow()
{
	//ShutDown();
	//DestroyWindow(hWnd);
	m_hWnd = nullptr;

	UnregisterClass(L"WindowClass1", hInstance);
	hInstance = nullptr;
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

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
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

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

void GameWindow::InitD3D()
{
	DirectX11::InitD3D(m_hWnd, screenWidth, screenHeight);
	DirectX11::InitD2D(m_hWnd);
	BuildWorldMatrix();
}

void GameWindow::GetWorl3DMatrix(XMMATRIX &m)
{
	m = world3DMatrix;
}

void GameWindow::GetWorl2DMatrix(XMMATRIX &m)
{
	m = world2DMatrix;
}

void GameWindow::GetOrtoM(XMMATRIX &m)
{
	m = orthoMatrix;
}

void GameWindow::GetProjectionM(XMMATRIX &m)
{
	m = projectionMatrix;
}

void GameWindow::BuildWorldMatrix()
{
	float fieldOfView, screenAspect;
	// Setup the projection matrix.
	fieldOfView = (float)XM_PI / m_screen.aspect;
	screenAspect = (float)m_screen.width / (float)m_screen.height;

	// Create the projection matrix for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screen.nearClip, m_screen.farClip);

	// Initialize the world matrix to the identity matrix.
	world3DMatrix = XMMatrixIdentity();
	world2DMatrix = world3DMatrix;
	world2DMatrix.r[2] = DirectX::XMVectorZero();
	//world2DMatrix.r[3] = DirectX::XMVectorZero();
	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = XMMatrixOrthographicLH((float)m_screen.width, (float)m_screen.height, +0.0f, 1);
}

HINSTANCE GameWindow::GetHinstance()
{
	return hInstance;
}

void GameWindow::SetWindowSize(LONG width, LONG heigth)
{
	m_screen.SetRez(width, heigth);
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

void GameWindow::ShowMessageBox(LPCWSTR msg)
{
	MessageBox(nullptr, msg, L"Critical Error", MB_OK);
}

void GameWindow::GetCursor(LPPOINT & poz)
{
	GetPhysicalCursorPos(poz);
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
