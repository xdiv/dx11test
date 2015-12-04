#include "pch.h"
#include "Game.h"
#include "ShaderModel.h"
#include "RayCast.h"

Game::Game(): gameWindow(nullptr), width (1280), heigth(720)
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
	gameWindow = new GameWindow(width, heigth, L"Hello world", 0.1F, 1000.0f);
	gameWindow->InitializeWindows();
	gameWindow->InitD3D();
}

void Game::Run()
{
	MSG msg;
	GameInit();
	//gameWindow->FullScreenSwitch(true);
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

	mouseLocation = new POINT();
	mouseLocation->x = 0;
	mouseLocation->y = 0;
	camera = new Camera();
	input = new ButtonsActionMap(gameWindow->GetHinstance(), gameWindow->GetHwnd());

	camera->SetPosition(0.0f, 0.0f, 0.0f);
	camera->SetRotation(-60.f, 0.f, 0.f);
	input->SetCamera(camera);

	normalShader = new NormalShader(gameWindow->GetD3D());
	normalShader->Init();

	gameWindow->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameWindow->TurnOnAlphaBlending();
	gameWindow->VSinc(true);

	dmb = 0;
	dmb = new DataModelBase();
	dmb->LoadTestModel3(gameWindow->GetD3DDevice());

	md5Model = 0;
	md5Model = new DataModelBase();
	md5Model->LoadTestModel4(gameWindow->GetD3DDevice());

	cubeModel = 0;
	cubeModel = new DataModelBase();
	cubeModel->LoadTestModelCube(gameWindow->GetD3DDevice());


	zz = 0;

	windowLocation = new RECT{ 0,0,0,0 };

	label = new Label(gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 0.0f, 100.0f, 100.0f }, D2D1_COLOR_F{ 1.f,0.f,0.f,1.f });
	mouseCLickLocation = new Label(gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 30.0f, 100.0f, 100.0f }, D2D1_COLOR_F{ 1.f,0.f,0.f,1.f });
	gameWindowLocation = new Label(gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 60.0f, 200.0f, 100.0f }, D2D1_COLOR_F{ 1.f,0.f,0.f,1.f });
	gameWindowLocation->TextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
}
void Game::GameShutDown()
{
	
	//SAFE_DELETE(label);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	SAFE_DELETE(md5Model);
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

	

	input->MouseCick([&]
	{
		gameWindow->GetCursor(mouseLocation);
		gameWindow->GetGameWindowRect(windowLocation);
		XMFLOAT3 x = RayCast::CameraToWorldRay(gameWindow->GetScreen(), *mouseLocation, viewMatrix, p._11, p._22);
		float l = Length(float3(-40, 0, 20));
		rz = XMFLOAT3{ x.x * l, x.y * l, x.z * l};
		BoundingBox box(XMFLOAT3(-40, 0, 20), XMFLOAT3(0, 0, 0));
		bool intersects = box.Intersects(XMLoadFloat3(&XMFLOAT3(0,0,0)), XMLoadFloat3(&x), l);
	});

	/*2d pasaulio cameros renderinimas, tikriausiai 
	dar reikia nustatye identity matricą į 2d*/
	camera->GetView2DMatrix(viewMatrix);
	gameWindow->GetOrtoM(p);
	world2Dmatrix = viewMatrix * p;
	gameWindow->GetWorl2DMatrix(w);
	gameWindow->GetOrtoM(p);
	interfaceMatrix = w * p;
	
	//xxf
	/*md5Model->AddInstance(InstanceType_B(float3(-15,  0, 65), color_rgba(1, 0, 0, 1),	float3(1), float3(-0.8f,0,0)));
	md5Model->AddInstance(InstanceType_B(float3(  40,  0, 20), color_rgba(0, 1, 0, 1),	float3(1, 1, 0.5)));
	md5Model->AddInstance(InstanceType_B(float3( 15,  65, 0), color_rgba(0, 0, 1, 1),	float3(1, 1, 1)));
*/
	cubeModel->AddInstance(InstanceType_B(float3(-40, 0, 20), color_rgba(0, 0, 1, 1), float3(1), float3(0, 0, 0)));
	
	label->SetText(std::to_wstring(timer.GetFramesPerSecond()) + L" FPS");
	mouseCLickLocation->SetText(std::to_wstring(mouseLocation->x) + L" - x " + std::to_wstring(mouseLocation->y) + L" - y ");
	/*gameWindowLocation->SetText(
		std::to_wstring(windowLocation->left) + L" - left " +
		std::to_wstring(windowLocation->top) + L" - top " +
		std::to_wstring(windowLocation->right) + L" - right " +
		std::to_wstring(windowLocation->bottom) + L" - bottom ");*/

	gameWindowLocation->SetText(
		std::to_wstring(rz.x) + L" - x " +
		std::to_wstring(rz.y) + L" - y " +
		std::to_wstring(rz.z) + L" - z ");
}
void Game::Render(StepTimer const& timer)
{
	PSConstBuffer ps;
	ps.color = float3(1, 0, 1);
	ps.hasColor = 0;
	ps.hasTexture = 1;
	ps.transperency = 1.0f;

	normalShader->SetVertexShaderBuffers(&world3DMatrix);
	normalShader->SetPixelShaderBuffers(&ps);


	md5Model->UpdateInstanceBuffer(gameWindow->GetD3DDeviceContext());
	cubeModel->UpdateInstanceBuffer(gameWindow->GetD3DDeviceContext());
	normalShader->Render(md5Model->GetData());
	normalShader->Render(cubeModel->GetData());
}

void Game::RenderInterface(StepTimer const& timer)
{
	//gameWindow->TurnOnAlphaBlending();
	//gameWindow->TurnZBufferOff();

	//gameWindow->GetD2DDeviceContext()->SaveDrawingState(m_stateBlock);
	gameWindow->GetD2DDeviceContext()->BeginDraw();
	//gameWindow->GetD2DDeviceContext()->SetTransform()
	label->Render();
	mouseCLickLocation->Render();
	gameWindowLocation->Render();

	gameWindow->GetD2DDeviceContext()->EndDraw();
	//gameWindow->TurnZBufferOn();
	//gameWindow->TurnOffAlphaBlending();
}