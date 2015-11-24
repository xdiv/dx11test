#include "pch.h"
#include "Game.h"

Game::Game(): gameWindow(nullptr), width (1200), heigth(900)
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

	GameWindow::SetInstance(gameWindow);
}

void Game::Run()
{
	MSG msg;
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

			m_timer.Tick([&] {
				Update(m_timer);
				Render(m_timer);
				RenderInterface(m_timer);
			});

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
	normalShader->Init(gameWindow->GetD3DDevice(), gameWindow->GetHwnd(), gameWindow->GetD3DDeviceContext());

	gameWindow->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameWindow->TurnOnAlphaBlending();
	gameWindow->VSinc(true);

	dmb = 0;
	dmb = new DataModelBase();
	dmb->LoadTestModel3(gameWindow->GetD3DDevice());

	xxf = 0;
	xxf = new DataModelBase();
	xxf->LoadTestModel4(gameWindow->GetD3DDevice());

	zz = 0;

	label = new Label(gameWindow->GetD2D(), L"Hello World", float4(0.0f, 0.0f, 100.0f, 100.0f), float4(1.f,0.f,0.f,1.f));
}
void Game::GameShutDown()
{
	
	//SAFE_DELETE(label);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(xxf);
	SAFE_DELETE(dmb);
	gameWindow->ShutDown();
}

void Game::Update(StepTimer const& timer)
{
	XMFLOAT4X4A abc;
	camera->Render3DCamera();
	camera->Render2DCamera();
	input->Update();

	gameWindow->GetWorl3DMatrix(w);
	gameWindow->GetProjectionM(p);
	/*3d pasaulio kameros renderinimas*/
	camera->GetView3DMatrix(viewMatrix);
	
	world3DMatrix = w * viewMatrix * p; 

	/*2d pasaulio cameros renderinimas, tikriausiai 
	dar reikia nustatye identity matricą į 2d*/
	camera->GetView2DMatrix(viewMatrix);
	gameWindow->GetOrtoM(p);
	world2Dmatrix = viewMatrix * p;
	gameWindow->GetWorl2DMatrix(w);
	gameWindow->GetOrtoM(p);
	interfaceMatrix = w * p;
	
	//xxf
	xxf->AddInstance(InstanceType_B(float3(-15, 15, 0), color_rgba(1, 0, 0, 1),		float3(1)));
	xxf->AddInstance(InstanceType_B(float3(0,	0,	0),	color_rgba(0, 1, 0, 1),	float3(1, 1, 0.5)));
	xxf->AddInstance(InstanceType_B(float3(15,	15, 0),	color_rgba(0, 0, 1, 1),	float3(1, 1, 1)));
	
	label->SetText(std::to_wstring(timer.GetFramesPerSecond()) + L" FPS");
}
void Game::Render(StepTimer const& timer)
{
	PSConstBuffer ps;
	ps.color = float3(1, 0, 1);
	ps.hasColor = 0;
	ps.hasTexture = 1;
	ps.transperency = 1.0f;

	normalShader->SetVertexShaderBuffers(gameWindow->GetD3DDeviceContext(), &world3DMatrix);
	normalShader->SetPixelShaderBuffers(gameWindow->GetD3DDeviceContext(), &ps);


	xxf->UpdateInstanceBuffer(gameWindow->GetD3DDeviceContext());
	normalShader->Render(gameWindow->GetD3DDeviceContext(), xxf->GetData());
}

void Game::RenderInterface(StepTimer const& timer)
{
	gameWindow->TurnOnAlphaBlending();
	gameWindow->TurnZBufferOff();

	//gameWindow->GetD2DDeviceContext()->SaveDrawingState(m_stateBlock);
	gameWindow->GetD2DDeviceContext()->BeginDraw();
	//gameWindow->GetD2DDeviceContext()->SetTransform()
	label->Render();

	gameWindow->GetD2DDeviceContext()->EndDraw();
	gameWindow->TurnZBufferOn();
	gameWindow->TurnOffAlphaBlending();
}