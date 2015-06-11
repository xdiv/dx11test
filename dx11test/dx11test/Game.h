#include "GameWindow.h"

#include "Camera.h"
#include "PublicData.h"
#include "test.h"
#include "TexturedModelBase.h"
#include "InstanedShader.h"
#include "itmr.h"
#include "ButtonsActionMap.h"
#include "InterfaceShader.h"
#include "test.h"

#pragma once
class Game
{
private:
	UINT width, heigth;
	GameWindow * gw;

private:
	void GameInit();
	void GameShutDown();
	void Update();
	void Render();
	void RenderInterface();
	

	D3DXMATRIX viewMatrix, finalMatrix;
	Camera* camera;
	ButtonsActionMap *input;
	itmr* insTest;
	InterfaceShader* is;
	test *tst;

public:
	Game();
	~Game();
	void Init();
	void Run();
};

//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

