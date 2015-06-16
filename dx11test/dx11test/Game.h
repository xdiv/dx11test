#pragma once
#ifndef __GAME__
#define __GAME__
#include "GameWindow.h"

#include "Camera.h"
#include "PublicData.h"
#include "ButtonsActionMap.h"
#include "ShadersIncludes.h"

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
	

	D3DXMATRIX viewMatrix, 
		world3DMatrix, world2Dmatrix, 
		interfaceMatrix;

	Camera* camera;
	ButtonsActionMap *input;
	NormalShader * nshad;

	DataModelBase* dmb, *xxf;

public:
	Game();
	~Game();
	void Init();
	void Run();
};

#endif

