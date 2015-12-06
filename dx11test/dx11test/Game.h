#pragma once
#ifndef __GAME__
#define __GAME__

#include "GameWindow.h"
#include "Camera.h"
#include "StepTimer.h"
#include "InputClass.h"

#include "ButtonsActionMap.h"
#include "ShadersIncludes.h"
#include "RayCast.h"

#include "GUI_Parts.h"

[event_receiver(native)]
class Game
{
public:
	Game();
	~Game();
	void Init();
	void Run();

private:
	Camera*				m_camera;
	GameWindow*			m_gameWindow;
	InputClass*			m_input;
	ButtonsActionMap*	m_buttonActions;
	StepTimer			m_timer;

	bool		m_running;

	LONG		m_width;
	LONG		m_heigth;

	XMMATRIX	m_viewMatrix;
	XMMATRIX	m_world3DMatrix;
	XMMATRIX	m_worldMatrix;
	XMMATRIX	m_projectionMatrix;

	XMFLOAT3	rz{0.f, 0.f, 0.f};

	NormalShader*	normalShader;
	DataModelBase*	md5Model; 
	DataModelBase*	cubeModel;
	Label*			label;
	Label*			mouseCLickLocation;
	Label*			gameWindowLocation;

	bool intersects;
	D2D1_RECT_F location ;

	LPPOINT mouseLocation;
	RECT* windowLocation;

	void GameInit();
	void GameShutDown();
	void Update(StepTimer const& timer);
	void Render(StepTimer const& timer);
	void RenderInterface(StepTimer const& timer);

	void ExitEventHandler() { m_running = false; }
	void MouseButton1ClickEventHandler()
	{
		Screen scr = m_gameWindow->GetScreen();
		XMFLOAT3 x = RayCast::CameraToWorldRay(m_gameWindow->GetScreen(), m_input->GetMouseLocation(), m_viewMatrix, m_projectionMatrix._11, m_projectionMatrix._22);
		float l = Length(float3(-40, 0, 20));
		rz = XMFLOAT3{ x.x * l, x.y * l, x.z * l };
		BoundingBox box(XMFLOAT3(-40, 0, 20), XMFLOAT3(.5f, .5f, .5f));
		intersects = box.Intersects(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&x), l);
		if (intersects)
		{
			XMFLOAT4X4 mx(
				-40.5f,  0.5f, 19.5f, 1.f,
				-39.5f, -0.5f, 19.5f, 1.f,
				-40.5f, -0.5f, 20.5f, 1.f,
				-39.5f,  0.5f, 20.5f, 1.f);

			XMMATRIX mm = XMLoadFloat4x4(&mx) * m_viewMatrix * m_projectionMatrix;

			XMStoreFloat4x4(&mx, mm);
			location.left	= (		 min(min(min(mx._11 / mx._14, mx._21 / mx._24), mx._31 / mx._34), mx._41 / mx._44) +1.f) * scr.widthHalf  - 2;
			location.right	= (		 max(max(max(mx._11 / mx._14, mx._21 / mx._24), mx._31 / mx._34), mx._41 / mx._44) +1.f) * scr.widthHalf  + 2;
			location.top	= (1.f - min(min(min(mx._12 / mx._14, mx._22 / mx._24), mx._32 / mx._34), mx._42 / mx._44))	*	   scr.heightHalf + 2;
			location.bottom	= (1.f - max(max(max(mx._12 / mx._14, mx._22 / mx._24), mx._32 / mx._34), mx._42 / mx._44)) *	   scr.heightHalf - 2;

			int i = 0;
		}
	}
};

#endif

