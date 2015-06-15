#pragma once

#ifndef __GAME_WINDOW__
#define __GAME_WINDOW__
#define WIN32_LEAN_AND_MEAN

#include "DX_Global.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

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

		D3DXMATRIX projectionMatrix;
		D3DXMATRIX world3DMatrix, world2DMatrix;
		D3DXMATRIX orthoMatrix;

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

		D3DXMATRIX GetWorl3DMatrix();
		D3DXMATRIX GetWorl2DMatrix();
		D3DXMATRIX GetProjectionM();
		D3DXMATRIX GetOrtoM();
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