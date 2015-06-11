#pragma once

#define WIN32_LEAN_AND_MEAN

#include "DX_Global.h"

#include "Camera.h"
#include "PublicData.h"
#include "test.h"
#include "TexturedModelBase.h"
#include "InstanedShader.h"
#include "itmr.h"
#include "ButtonsActionMap.h"
#include "InterfaceShader.h"
#include "test.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class GameWindow
{
	private:
		int m_videoCardMemory;
		int screenWidth, screenHeight;
		float screenNear, screenDepth;

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

		D3DXMATRIX viewMatrix, finalMatrix;
		Camera* camera;
		ButtonsActionMap *input;
		itmr* insTest;
		InterfaceShader* is;
		test *tst;

		char m_videoCardDescription[128];

		D3DXMATRIX projectionMatrix;
		D3DXMATRIX worldMatrix;
		D3DXMATRIX orthoMatrix;
	public:
		GameWindow(int&, int&, LPCWSTR, float screenNear, float screenDepth);
		~GameWindow();
		void InitializeWindows();
		void InitD3D();
		void Close();
		void Run();
		void ShutDown();
		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		void GameInit();
		void GameShutDown();
		void Update();
		void Render();
		void RenderInterface();
};

LRESULT CALLBACK WindowProc(HWND hWnd, 	UINT message, WPARAM wParam, LPARAM lParam);

