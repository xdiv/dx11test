#pragma once
#ifndef __GAME__
#define __GAME__
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

class Game
{
private:
	LONG width, heigth;
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

#endif

