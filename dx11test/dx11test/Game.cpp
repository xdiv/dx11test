﻿#include "Game.h"
#include <math.h>

Game::Game(): gameWindow(nullptr), width (800), heigth(600)
{
}


Game::~Game()
{
	if (gameWindow)
	{
		gameWindow->ShutDown();
	}
	SAFE_DELETE(gameWindow);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
}

void Game::Init()
{
	gameWindow = new GameWindow(width, heigth, L"Hello world", 0.1F, 100000.0f, 4);
	gameWindow->InitializeWindows();
	gameWindow->InitD3D();

	time.Initialize();

	GameWindow::SetInstance(gameWindow);
}

void Game::Run()
{
	MSG msg;
	//test * t = new test(dev, hWnd, devcon, D3DXVECTOR3(0, 0, 0));
	GameInit();
	while (TRUE)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			gameWindow->BeginScene();

			Update();
			Render();
			//t->Render(devcon, worldMatrix, viewMatrix, projectionMatrix);
			RenderInterface();

			gameWindow->EndScene();
		}

	}

	GameShutDown();
}

void Game::GameInit()
{
	//ShowCursor(FALSE);
	camera = new Camera();
	input = new ButtonsActionMap(gameWindow->GetHinstance(), gameWindow->GetHwnd());

	camera->SetPosition(0.0f, -50.0f, 0.0f);
	input->SetCamera(camera);

	normalShader = new NormalShader();
	normalShader->Init(gameWindow->GetDevice(), gameWindow->GetHwnd(), gameWindow->GetDeviceContext());

	gameWindow->GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameWindow->TurnOnAlphaBlending();
	gameWindow->VSinc(true);

	dmb = 0;
	dmb = new DataModelBase();
	dmb->LoadTestModel3(gameWindow->GetDevice());

	xxf = 0;
	xxf = new DataModelBase();
	xxf->LoadTestModel4(gameWindow->GetDevice());

	zz = 0;
}
void Game::GameShutDown()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(xxf);
	SAFE_DELETE(dmb);
}

void Game::Update()
{
	XMFLOAT4X4A abc;
	time.Frame();
	time.GetTime();
	camera->Render3DCamera();
	camera->Render2DCamera();
	input->Update();

	gameWindow->GetWorl3DMatrix(w);
	gameWindow->GetProjectionM(p);
	/*3d pasaulio kameros renderinimas*/
	camera->GetView3DMatrix(viewMatrix);
	//XMMATRIX()
	
	world3DMatrix = XMMatrixMultiply(XMMatrixMultiply(w, viewMatrix), p);


	/*2d pasaulio cameros renderinimas, tikriausiai 
	dar reikia nustatye identity matricą į 2d*/
	camera->GetView2DMatrix(viewMatrix);
	gameWindow->GetOrtoM(p);
	world2Dmatrix = viewMatrix * p;
	gameWindow->GetWorl2DMatrix(w);
	gameWindow->GetOrtoM(p);
	interfaceMatrix = w * p;

	//dmb->AddInstance(InstanceType_B(float3( -150, -150, 0),		color_rgba(1, 0, 0, 1),		float3(100, 50, 1)));
	//dmb->AddInstance(InstanceType_B(float3(	0,		0,	0),		color_rgba(0, 1, 0, 0.5),	float3(50,  100, 1)));
	//ff += 0.1 * time.GetTime();
	ff = 20;
	for (size_t i = 0; i < 20; i++)
	//while (zz < 1)
	{
		if (alpha > 2)
		{
			alpha -= 2;
		}

		if (zz > 2)
			zz -= 2;

		float rr = alpha +zz;
		if (rr > 2)
			rr = rr -2;

		float rot = PI * 0.5 + PI*rr * 0.5;// +PI * zz * 0.5;

		float tt = 0;
		tt = sin(rot);
		float4 q = float4(0, 0, tt, 0);
		float3 a = float3(ff, 0, 0);
		ComputeQuatW(q);
		a = Multiply(q, a);
		dmb->AddInstance(InstanceType_B(a, color_rgba(1, 0, 0, 1), float3(2, 2, 1)));
		zz += 0.1f;
	}
	zz = 0;
	//for (size_t i = 0; i < 2; i++)
	////while (zz < 1)
	//{
	//	float4 q = float4(0, 0, zz, 0);
	//	float3 a = float3(ff, 0, 0);
	//	ComputeQuatW(q);
	//	a = Multiply(q, a);
	//	dmb->AddInstance(InstanceType_B(a, color_rgba(1, 1, 0, 1), float3(5, 5, 1)));
	//	zz += 0.5f;
	//}

	//zz = -1;
	//for (size_t i = 0; i < 2; i++)
	//	//while (zz < 1)
	//{
	//	float4 q = float4(0, 0, zz, 0);
	//	float3 a = float3(ff, 0, 0);
	//	ComputeQuatW(q);
	//	a = Multiply(q, a);
	//	dmb->AddInstance(InstanceType_B(a, color_rgba(0, 1, 1, 1), float3(5, 5, 1)));
	//	zz += 0.5f;
	//}

	//zz = -1;
	if (ff > 400)
		ff = -1;
	//dmb->AddInstance(InstanceType_B(float3(-50, 50, 0), color_rgba(1, 0, 0, 1), float3(200, 1, 1)));
	dmb->AddInstance(InstanceType_B(float3(0, 0, 0), color_rgba(0, 1, 0, 1), float3(500, 1, 1), float3(0.0f, 0.0f, 0) ));
	dmb->AddInstance(InstanceType_B(float3(0, 0, 0), color_rgba(0, 1, 0, 1), float3(1, 500, 1), float3(0.0f, 0.0f, 0)));

	alpha += 0.004; //* time.GetTime();
	
	
	//xxf
	xxf->AddInstance(InstanceType_B(float3(-15, 15, 0), color_rgba(1, 0, 0, 1),		float3(1)));
	xxf->AddInstance(InstanceType_B(float3(0,	0,	0),	color_rgba(0, 1, 0, 1),	float3(1, 1, 0.5)));
	xxf->AddInstance(InstanceType_B(float3(15,	15, 0),	color_rgba(0, 0, 1, 1),	float3(1, 1, 1)));
}
void Game::Render()
{
	PSConstBuffer ps;
	ps.color = float3(1, 0, 1);
	ps.hasColor = 0;
	ps.hasTexture = 1;
	ps.transperency = 1.0f;

	normalShader->SetVertexShaderBuffers(gameWindow->GetDeviceContext(), &world3DMatrix);
	normalShader->SetPixelShaderBuffers(gameWindow->GetDeviceContext(), &ps);


	xxf->UpdateInstanceBuffer(gameWindow->GetDeviceContext());
	normalShader->Render(gameWindow->GetDeviceContext(), xxf->GetData());
}

void Game::RenderInterface()
{
	//gameWindow->TurnOnAlphaBlending();
	//gameWindow->TurnZBufferOff();

	//PSConstBuffer ps;
	//ps.color = float3(1, 1, 1);
	//ps.hasColor = 1;
	//ps.hasTexture = 0;
	//ps.transperency = 0.5f;

	//normalShader->SetVertexShaderBuffers(gameWindow->GetDeviceContext(), &interfaceMatrix);
	//normalShader->SetPixelShaderBuffers(gameWindow->GetDeviceContext(), &ps);

	///*pvz renderinti statini objektą*/
	//dmb->UpdateInstanceBuffer(gameWindow->GetDeviceContext());
	//normalShader->Render(gameWindow->GetDeviceContext(), dmb->GetData());

	//
	//gameWindow->TurnZBufferOn();
	//gameWindow->TurnOffAlphaBlending();
}