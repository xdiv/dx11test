#pragma once

#define WIN32_LEAN_AND_MEAN

#include "DX_Global.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class GameWindow
{
	private:
		int m_videoCardMemory;
		static LONG screenWidth, screenHeight;
		float screenNear, screenDepth;
		bool vsync_enabled;
		HWND hWnd;
		HINSTANCE hInstance;

		LPCWSTR title;
		IDXGISwapChain *swapchain;
		ID3D11Device *dev;
		ID3D11DeviceContext *devcon;
		ID3D11RenderTargetView *backbuffer;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RasterizerState* rasterState;
		ID3D11BlendState *alphaEnableBlendingState, *alphaDisableBlendingState;
		//renderTargetView

		char m_videoCardDescription[128];

		D3DXMATRIX projectionMatrix;
		D3DXMATRIX worldMatrix;
		D3DXMATRIX orthoMatrix;
	protected:
		GameWindow(GameWindow&){};
	public:
		GameWindow(LONG&, LONG&, LPCWSTR, float screenNear, float screenDepth);
		~GameWindow();
		void InitializeWindows();
		void InitD3D();
		void ShutDown();
		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		D3DXMATRIX GetWorlM();
		D3DXMATRIX GetProjectionM();
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		HWND GetHwnd();
		HINSTANCE GetHinstance();

		void BeginScene();
		void EndScene();
		void VSinc(bool state);
		static void NewGameWindowSize(LONG width, LONG heigth);
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

