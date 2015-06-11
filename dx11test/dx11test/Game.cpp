#include "Game.h"

Game::Game()
{
	gw = 0;
	width = 800;
	heigth = 600;
	tst = 0;
}


Game::~Game()
{
	if (gw)
	{
		gw->ShutDown();
	}

	SAFE_DELETE(gw);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(insTest);
	SAFE_DELETE(is);
	SAFE_DELETE(tst);
}

void Game::Init()
{
	gw = new GameWindow(width, heigth, L"Hello world", 0.1F, 1000.0f);
	gw->InitializeWindows();
	gw->InitD3D();
}

void Game::Run()
{
	MSG msg;
	//test * t = new test(dev, hWnd, devcon, D3DXVECTOR3(0, 0, 0));
	GameInit();
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
			gw->BeginScene();

			Update();
			Render();
			//t->Render(devcon, worldMatrix, viewMatrix, projectionMatrix);
			RenderInterface();

			gw->EndScene();
		}

	}

	GameShutDown();
}

void Game::GameInit()
{
	//ShowCursor(FALSE);
	camera = new Camera();
	input = new ButtonsActionMap(gw->GetHinstance(), gw->GetHwnd());

	camera->SetPosition(0.0f, -50.0f, 3.0f);
	input->SetCamera(camera);

	insTest = new itmr();
	insTest->Init(gw->GetDevice(), gw->GetHwnd(), gw->GetDeviceContext());

	is = new InterfaceShader();
	is->Init(gw->GetDevice(), gw->GetHwnd(), gw->GetDeviceContext());

	tst = new test(gw->GetDevice(), gw->GetHwnd(), gw->GetDeviceContext(), D3DXVECTOR3(0, 0, 0));

	gw->GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gw->TurnOnAlphaBlending();
	gw->VSinc(true);
}
void Game::GameShutDown()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(insTest);
}

void Game::Update()
{
	camera->Render();
	input->Update();
	camera->GetViewMatrix(viewMatrix);
	finalMatrix = gw->GetWorlM() * viewMatrix * gw->GetProjectionM();

	insTest->AddInstance(InstanceType_A(0, 0, 0));
	insTest->AddInstance(InstanceType_A(15, 15, 0));
	//insTest->AddInstance(InstanceType_A(-15, 0, 0));
}
void Game::Render()
{
	insTest->Render(gw->GetDeviceContext(), finalMatrix);
}

void Game::RenderInterface()
{
	gw->TurnOnAlphaBlending();
	PSConstBuffer ps;
	ps.color = float3(1, 1, 1);
	ps.hasColor = 0;
	ps.hasTexture = 0;
	ps.transperency = 0.5f;
	is->Render(gw->GetDeviceContext(), float4(0, 0, 1, 1), NULL, ps);
	gw->TurnOffAlphaBlending();
}

//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// sort through and find what code to run for the message given
//	switch (message)
//	{
//		// this message is read when the window is closed
//	case WM_DESTROY:
//	{
//		// close the application entirely
//		PostQuitMessage(0);
//		return 0;
//	} break;
//	case WM_SIZE:
//	{
//		RECT rect;
//		if (GetWindowRect(hWnd, &rect))
//		{
//			int width = rect.right - rect.left;
//			int height = rect.bottom - rect.top;
//		}
//	} break;
//	}
//
//	// Handle any messages the switch statement didn't
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}