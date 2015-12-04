#pragma once

#ifndef __GAME_WINDOW__
#define __GAME_WINDOW__
#define WIN32_LEAN_AND_MEAN

#include "DirectX11.h"
#include "IScreen.h"

using namespace DirectX;

class GameWindow : public DirectX11, public IScreen
{
	public:
		GameWindow(LONG&, LONG&, LPCWSTR, float screenNear, float screenDepth);
		~GameWindow();
		void InitializeWindows();
		void InitD3D();

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
		void GetCursor(LPPOINT poz);

		// Inherited via IScreen
		Screen		GetScreen() override;
		HINSTANCE	GetHinstance() override;
		bool		GetGameWindowRect(LPRECT lpRect) override;
private:
	XMMATRIX	projectionMatrix;
	XMMATRIX	world3DMatrix;
	XMMATRIX	world2DMatrix;
	XMMATRIX	orthoMatrix;

	Screen		m_screen;
	HINSTANCE	hInstance;
	LPCWSTR		title;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//GameWindow* pb;
#endif