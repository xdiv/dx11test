#include "pch.h"
#include "Game.h"
#include "ShaderModel.h"

Game::Game(): m_gameWindow(nullptr), m_width (1280), m_heigth(720)
{
}


Game::~Game()
{
	if (m_gameWindow)
	{
		m_gameWindow->ShutDown();
	}
	SAFE_DELETE(m_gameWindow);
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_buttonActions);
}

void Game::Init()
{
	m_gameWindow = new GameWindow(m_width, m_heigth, L"Hello world", 0.1F, 1000.0f);
	m_gameWindow->InitializeWindows();
	m_gameWindow->InitD3D();
}

void Game::Run()
{
	MSG msg;
	GameInit();
	m_running = true;
	//gameWindow->FullScreenSwitch(true);
	while (m_running)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			ExitEventHandler();
		}
		else
		{
			m_gameWindow->BeginScene();

			m_timer.Tick([&] {
				Update(m_timer);
				Render(m_timer);
				RenderInterface(m_timer);
			});

			m_gameWindow->EndScene();
		}

	}
	GameShutDown();
}

void Game::GameInit()
{
	intersects = false;
	location = {200.f, 200.f, 400.f, 400.f };
	m_camera = new Camera();
	m_camera->SetPosition(0.0f, 0.0f, 0.0f);
	m_camera->SetRotation(-60.f, 0.f, 0.f);

	m_input = new InputClass((IScreen*)m_gameWindow);
	m_input->Initialize();
	m_buttonActions = new ButtonsActionMap(m_input);
	
	__hook(&ButtonsActionMap::ExitEvent, m_buttonActions, &Game::ExitEventHandler);
	__hook(&ButtonsActionMap::MouseButton1ClickEvent, m_buttonActions, &Game::MouseButton1ClickEventHandler);

	normalShader = new NormalShader(m_gameWindow->GetD3D());
	normalShader->Init();

	m_gameWindow->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_gameWindow->TurnOnAlphaBlending();
	m_gameWindow->VSinc(true);

	md5Model = 0;
	md5Model = new DataModelBase();
	md5Model->LoadTestModel4(m_gameWindow->GetD3DDevice());

	cubeModel = 0;
	cubeModel = new DataModelBase();
	cubeModel->LoadTestModelCube(m_gameWindow->GetD3DDevice());

	windowLocation = new RECT{ 0,0,0,0 };

	label = new Label(m_gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 0.0f, 100.0f, 100.0f }, D2D1_COLOR_F{ 1.f,0.f,0.f,1.f });
	mouseCLickLocation = new Label(m_gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 30.0f, 100.0f, 100.0f }, D2D1_COLOR_F{ 0.909803f, 0.635294f, 0.f, 1.f });
	gameWindowLocation = new Label(m_gameWindow->GetD2D(), L"Hello World", Rect_F{ 0.0f, 60.0f, 200.0f, 100.0f }, D2D1_COLOR_F{ 0.f, 0.635294f, 0.909803f, 1.f });
	gameWindowLocation->TextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	m_gameWindow->GetWorl3DMatrix(m_worldMatrix);
	m_gameWindow->GetProjectionM(m_projectionMatrix);
}
void Game::GameShutDown()
{
	__unhook(&ButtonsActionMap::ExitEvent, m_buttonActions, &Game::ExitEventHandler);
	__unhook(&ButtonsActionMap::MouseButton1ClickEvent, m_buttonActions, &Game::MouseButton1ClickEventHandler);
	//SAFE_DELETE(label);
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_buttonActions);
	SAFE_DELETE(md5Model);
	m_gameWindow->ShutDown();
}

void Game::Update(StepTimer const& timer)
{
	m_camera->Render3DCamera();
	m_camera->Render2DCamera();
	m_input->Update();
	m_buttonActions->Update();

	mouseLocation = m_input->GetMouseLocation();

	m_camera->GetView3DMatrix(m_viewMatrix);
	
	m_world3DMatrix = m_worldMatrix * m_viewMatrix * m_projectionMatrix;
	
	//xxf
	/*md5Model->AddInstance(InstanceType_B(float3(-15,  0, 65), color_rgba(1, 0, 0, 1),	float3(1), float3(-0.8f,0,0)));
	md5Model->AddInstance(InstanceType_B(float3(  40,  0, 20), color_rgba(0, 1, 0, 1),	float3(1, 1, 0.5)));
	md5Model->AddInstance(InstanceType_B(float3( 15,  65, 0), color_rgba(0, 0, 1, 1),	float3(1, 1, 1)));
*/
	cubeModel->AddInstance(InstanceType_B(float3(-40, 0, 20), color_rgba(0, 0, 1, 1), float3(1), float3(0, 0, 0)));
	
	label->SetText(std::to_wstring(timer.GetFramesPerSecond()) + L" FPS");
	//label->SetText(std::_Floating_to_wstring(L"%.5f", timer.GetElapsedSeconds()) + L" ms");
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

	normalShader->SetVertexShaderBuffers(&m_world3DMatrix);
	normalShader->SetPixelShaderBuffers(&ps);


	md5Model->UpdateInstanceBuffer(m_gameWindow->GetD3DDeviceContext());
	cubeModel->UpdateInstanceBuffer(m_gameWindow->GetD3DDeviceContext());
	normalShader->Render(md5Model->GetData());
	normalShader->Render(cubeModel->GetData());
}

void Game::RenderInterface(StepTimer const& timer)
{
	//gameWindow->TurnOnAlphaBlending();
	//gameWindow->TurnZBufferOff();

	//gameWindow->GetD2DDeviceContext()->SaveDrawingState(m_stateBlock);
	m_gameWindow->GetD2DDeviceContext()->BeginDraw();
	//gameWindow->GetD2DDeviceContext()->SetTransform()
	label->Render();
	mouseCLickLocation->Render();
	gameWindowLocation->Render();
	if (intersects)
	{
		m_gameWindow->GetD2DDeviceContext()->DrawRectangle(location, (ID2D1Brush*)gameWindowLocation->GetBrush(), 1.f);
	}

	m_gameWindow->GetD2DDeviceContext()->EndDraw();
	//gameWindow->TurnZBufferOn();
	//gameWindow->TurnOffAlphaBlending();
}