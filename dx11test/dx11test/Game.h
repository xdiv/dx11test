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

	XMFLOAT3 rz{0.f, 0.f, 0.f};

	Camera* camera;
	ButtonsActionMap *input;
	NormalShader * normalShader;

	DataModelBase* dmb, *md5Model, *cubeModel;

	StepTimer m_timer;
	Label* label;
	Label* mouseCLickLocation;
	Label* gameWindowLocation;

	float zz = -1.0f;
	float ff = 0;
	float alpha = 0;

	LPPOINT mouseLocation;
	LPRECT windowLocation;

	void GameInit();
	void GameShutDown();
	void Update(StepTimer const& timer);
	void Render(StepTimer const& timer);
	void RenderInterface(StepTimer const& timer);
};

#endif

