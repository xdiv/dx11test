#include "Game.h"

Game::Game()
{
	gw = 0;
	width = 800;
	heigth = 600;
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
}

void Game::Init()
{
	gw = new GameWindow(width, heigth, L"Hello world", 0.1F, 1000.0f, 4.0f);
	gw->InitializeWindows();
	gw->InitD3D();

	GameWindow::SetInstance(gw);
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

	nshad = new NormalShader();
	nshad->Init(gw->GetDevice(), gw->GetHwnd(), gw->GetDeviceContext());

	gw->GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gw->TurnOnAlphaBlending();
	gw->VSinc(true);

	dmb = 0;
	dmb = new DataModelBase();
	dmb->LoadTestModel1(gw->GetDevice());

	xxf = 0;
	xxf = new DataModelBase();
	xxf->LoadTestModel2(gw->GetDevice());
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
	camera->Render3DCamera();
	camera->Render2DCamera();
	input->Update();

	/*3d pasaulio kameros renderinimas*/
	camera->GetView3DMatrix(viewMatrix);
	world3DMatrix = gw->GetWorl3DMatrix() * viewMatrix * gw->GetProjectionM();

	/*2d pasaulio cameros renderinimas, tikriausiai 
	dar reikia nustatye identity matricą į 2d*/
	camera->GetView2DMatrix(viewMatrix);
	world2Dmatrix = viewMatrix * gw->GetOrtoM();

	interfaceMatrix = gw->GetWorl2DMatrix() * gw->GetOrtoM();

	//dmb->AddInstance(InstanceType_B(float3( -150, -150, 0),		color_rgba(1, 0, 0, 1),		float3(100, 50, 1)));
	//dmb->AddInstance(InstanceType_B(float3(	0,		0,	0),		color_rgba(0, 1, 0, 0.5),	float3(50,  100, 1)));
	zz += 0.01f;
	dmb->AddInstance(InstanceType_B(float3(50, 50, 0), color_rgba(0, 0, 1, 0.5), float3(100, 100, 1), float3(0.0f, 0.0f, zz) ));

	
	if (zz > 1.0f)
		zz = -1.0f;
	//xxf
	xxf->AddInstance(InstanceType_B(float3(-15, 15, 0), color_rgba(1, 0, 0, 1),		float3(2)));
	xxf->AddInstance(InstanceType_B(float3(0,	0,	0),	color_rgba(0, 1, 0, 0.5),	float3(1, 1, 0.5)));
	xxf->AddInstance(InstanceType_B(float3(15,	15, 0),	color_rgba(0, 0, 1, 0.5),	float3(1, 1, 1)));
}
void Game::Render()
{
	PSConstBuffer ps;
	ps.color = float3(1, 1, 1);
	ps.hasColor = 0;
	ps.hasTexture = 1;
	ps.transperency = 1.0f;

	nshad->SetVertexShaderBuffers(gw->GetDeviceContext(), &world3DMatrix);
	nshad->SetPixelShaderBuffers(gw->GetDeviceContext(), &ps);


	xxf->UpdateInstanceBuffer(gw->GetDeviceContext());
	nshad->Render(gw->GetDeviceContext(), xxf->GetData());
}

void Game::RenderInterface()
{
	gw->TurnZBufferOff();
	gw->TurnOnAlphaBlending();

	PSConstBuffer ps;
	ps.color = float3(1, 1, 1);
	ps.hasColor = 1;
	ps.hasTexture = 0;
	ps.transperency = 0.5f;

	nshad->SetVertexShaderBuffers(gw->GetDeviceContext(), &interfaceMatrix);
	nshad->SetPixelShaderBuffers(gw->GetDeviceContext(), &ps);

	/*pvz renderinti statini objektą*/
	dmb->UpdateInstanceBuffer(gw->GetDeviceContext());
	nshad->Render(gw->GetDeviceContext(), dmb->GetData());

	gw->TurnOffAlphaBlending();
	gw->TurnZBufferOn();
}