#pragma once
#ifndef __GAME__
#define __GAME__
#include "GameWindow.h"

#include "Camera.h"
#include "ButtonsActionMap.h"
#include "ShadersIncludes.h"

#include "Timer.h"

const float PI = 3.14159265;

class Game
{
private:
	LONG width, heigth;
	GameWindow * gameWindow;

private:
	void GameInit();
	void GameShutDown();
	void Update();
	void Render();
	void RenderInterface();
	

	XMMATRIX viewMatrix,
		world3DMatrix, world2Dmatrix,
		interfaceMatrix;
	XMMATRIX w, p;

	Camera* camera;
	ButtonsActionMap *input;
	NormalShader * normalShader;

	DataModelBase* dmb, *xxf;

	Timer time;

	float zz = -1.0f;
	float ff = 0;
	float alpha = 0;

public:
	Game();
	~Game();
	void Init();
	void Run();
};

#endif

