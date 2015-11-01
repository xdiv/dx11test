#pragma once

#ifndef __GAME_WINDOW__
#define __GAME_WINDOW__
#define WIN32_LEAN_AND_MEAN

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
#endif

using namespace DirectX;

class GameWindow
{
	private:
		int m_videoCardMemory;
		LONG screenWidth, screenHeight;
		float screenNear, screenDepth, aspectRatio;
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
		ID3D11DepthStencilState* depthDisabledStencilState;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RasterizerState* rasterState;
		ID3D11BlendState *alphaEnableBlendingState, *alphaDisableBlendingState;
		//renderTargetView

		char m_videoCardDescription[128];

		XMMATRIX projectionMatrix;
		XMMATRIX world3DMatrix, world2DMatrix;
		XMMATRIX orthoMatrix;

		static GameWindow * sInst;
	protected:
		GameWindow(GameWindow&){};
	public:
		GameWindow(LONG&, LONG&, LPCWSTR, float screenNear, float screenDepth, float aspectRatio);
		~GameWindow();
		void InitializeWindows();
		void InitD3D();
		void ShutDown();
		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		XMMATRIX GetWorl3DMatrix();
		XMMATRIX GetWorl2DMatrix();
		XMMATRIX GetProjectionM();
		XMMATRIX GetOrtoM();
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		HWND GetHwnd();
		HINSTANCE GetHinstance();

		void BeginScene();
		void EndScene();
		void VSinc(bool state);
		void BuildWorldMatrix();
		void SetWindowSize(LONG width, LONG heigth);
		void SetAspectRatio(float aspectRatio);

		void TurnZBufferOn();
		void TurnZBufferOff();

		static void SetInstance(GameWindow * gw);
		static GameWindow * GetInstance();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//GameWindow* pb;
#endif