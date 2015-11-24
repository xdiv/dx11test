#pragma once

#ifndef __GAME_WINDOW__
#define __GAME_WINDOW__
#define WIN32_LEAN_AND_MEAN

#include "DirectX11.h"

using namespace DirectX;

class GameWindow : public DirectX11
{
	private:
		int m_videoCardMemory;
		LONG screenWidth, screenHeight;
		float screenNear, screenDepth, aspectRatio;
		
		HINSTANCE hInstance;

		LPCWSTR title;
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
		
		HWND GetHwnd();
		HINSTANCE GetHinstance();

		/// <summary>Gražina 3d pasaulio matrica
		/// <para>gražinama matrica</para>
		/// </summary>
		void GetWorl3DMatrix(XMMATRIX &);

		/// <summary>Gražina 2d pasaulio matrica
		/// <para>gražinama matrica</para>
		/// </summary>
		void GetWorl2DMatrix(XMMATRIX &);
		void GetProjectionM(XMMATRIX &);
		void GetOrtoM(XMMATRIX&);
		void BuildWorldMatrix();
		
		void SetWindowSize(LONG width, LONG heigth);
		static void ShowMessageBox(LPCWSTR msg);
		void GetCursor(LPPOINT& poz);
		static void SetInstance(GameWindow * gw);
		static GameWindow * GetInstance();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//GameWindow* pb;
#endif