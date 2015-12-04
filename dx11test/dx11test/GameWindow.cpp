#include "pch.h"
#include "GameWindow.h"

#define WINDOW_WIDTH_PAD  16
#define WINDOW_HEIGHT_PAD 39

GameWindow::GameWindow(LONG& w, LONG& h, LPCWSTR t, float screenNear, float screenDepth)
	: m_screen(w, h)
{
	title = t;
	hInstance = 0;

	m_screen.SetRez(w, h);
	m_screen.SetClip(screenNear, screenDepth);
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

	POINT location;
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	location.x = (desktop.right - m_screen.width + WINDOW_WIDTH_PAD) * .5f;
	location.y = (desktop.bottom - m_screen.height + WINDOW_HEIGHT_PAD) * .5f;

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"WindowClass1",    // name of the window class
		title,   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		location.x,    // x-position of the window
		location.y,    // y-position of the window
		m_screen.width + WINDOW_WIDTH_PAD,    // width of the window
		m_screen.height + WINDOW_HEIGHT_PAD,    // height of the window
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
	DirectX11::InitD3D(m_hWnd, m_screen.width, m_screen.height);
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
	fieldOfView = (float)XM_PI / 4.f;

	// Create the projection matrix for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, m_screen.aspect, m_screen.nearClip, m_screen.farClip);

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

Screen GameWindow::GetScreen()
{
	RECT rect;
	UINT w, h;

	GetClientRect(this->m_hWnd, &rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
	if (w != m_screen.width || h != m_screen.height)
		m_screen.SetRez(w, h);
	m_screen.x = rect.left;
	m_screen.y = rect.top;
	return m_screen;
}

bool GameWindow::GetGameWindowRect(LPRECT lpRect)
{
	return GetWindowRect(this->m_hWnd, lpRect);
}

void GameWindow::ShowMessageBox(LPCWSTR msg)
{
	MessageBox(nullptr, msg, L"Critical Error", MB_OK);
}

void GameWindow::GetCursor(LPPOINT poz)
{
	GetPhysicalCursorPos(poz);
	ScreenToClient(m_hWnd, poz);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_CLOSE:
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_SIZE:
		{

		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
