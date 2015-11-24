#pragma once
#ifndef __GAME__
#define __GAME__
#include "GameWindow.h"

#include "Camera.h"
#include "ButtonsActionMap.h"
#include "ShadersIncludes.h"

#include "StepTimer.h"
#include "GUI_Parts.h"

const float PI = 3.14159265;

class Game
{
public:
	Game();
	~Game();
	void Init();
	void Run();

private:
	LONG width, heigth;
	GameWindow * gameWindow;

	XMMATRIX viewMatrix,
		world3DMatrix, world2Dmatrix,
		interfaceMatrix;
	XMMATRIX w, p;

	Camera* camera;
	ButtonsActionMap *input;
	NormalShader * normalShader;

	DataModelBase* dmb, *xxf;

	StepTimer m_timer;
	Label* label;

	float zz = -1.0f;
	float ff = 0;
	float alpha = 0;

	void GameInit();
	void GameShutDown();
	void Update(StepTimer const& timer);
	void Render(StepTimer const& timer);
	void RenderInterface(StepTimer const& timer);
};

#endif

