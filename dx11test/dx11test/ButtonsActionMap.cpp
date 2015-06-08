#include "ButtonsActionMap.h"

ButtonsActionMap::ButtonsActionMap(HINSTANCE hInstance, HWND hWnd)
{
	input = 0;
	input = new InputClass();
	input->Initialize(hInstance, hWnd);

	moveForw  = KEY_W;
	moveBack  = KEY_S;
	moveLeft  = KEY_A;
	moveRigth = KEY_D;
	moveUp	  = KEY_SPACE;
	moveDow	  = KEY_LCONTROL;
	exit	  = KEY_ESCAPE;
}


ButtonsActionMap::~ButtonsActionMap()
{
	SAFE_DELETE(input);
}

void ButtonsActionMap::Update()
{
	input->Frame();

	MoveForward();
	MoveBack();
	MoveLeft();
	MoveRight();
	MoveUp();
	MoveDown();
}


void ButtonsActionMap::SetCamera(Camera* cam)
{
	camera = cam;
}

void ButtonsActionMap::MoveForward()
{
	if(input->KeyPressedDown(KEY_W) || input->KeyHoldDown(KEY_W))
		camera->MoveCamera(0.0f, 0.2f, 0.0f);
}

void ButtonsActionMap::MoveBack()
{
	if(input->KeyPressedDown(KEY_S) || input->KeyHoldDown(KEY_S))
		camera->MoveCamera(0.0f, -0.2f, 0.0f);
}

void ButtonsActionMap::MoveLeft()
{
	if(input->KeyPressedDown(KEY_A) || input->KeyHoldDown(KEY_A))
		camera->MoveCamera(0.2f, 0.0f, 0.0f);
}

void ButtonsActionMap::MoveRight()
{
	if(input->KeyPressedDown(KEY_D) || input->KeyHoldDown(KEY_D))
		camera->MoveCamera(-0.2f, 0.0f, 0.0f);
}

void ButtonsActionMap::MoveUp()
{
	if(input->KeyPressedDown(KEY_SPACE) || input->KeyHoldDown(KEY_SPACE))
		camera->MoveCamera(0.0f, 0.0f, 0.2f);
}

void ButtonsActionMap::MoveDown()
{
	if(input->KeyPressedDown(KEY_LCONTROL) || input->KeyHoldDown(KEY_LCONTROL))
		camera->MoveCamera(0.0f, 0.0f, -0.2f);
}
