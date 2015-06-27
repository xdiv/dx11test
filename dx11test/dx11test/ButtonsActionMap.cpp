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
	moveDow	  = KEY_C;
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
	Rotate();
}


void ButtonsActionMap::SetCamera(Camera* cam)
{
	camera = cam;
}

void ButtonsActionMap::MoveForward()
{
	if (input->KeyPressedDown(moveForw) || input->KeyHoldDown(moveForw))
		camera->MoveCamera(0.0f, 0.2f, 0.0f);
}

void ButtonsActionMap::MoveBack()
{
	if (input->KeyPressedDown(moveBack) || input->KeyHoldDown(moveBack))
		camera->MoveCamera(0.0f, -0.2f, 0.0f);
}

void ButtonsActionMap::MoveLeft()
{
	if (input->KeyPressedDown(moveLeft) || input->KeyHoldDown(moveLeft))
		camera->MoveCamera(0.2f, 0.0f, 0.0f);
}

void ButtonsActionMap::MoveRight()
{
	if (input->KeyPressedDown(moveRigth) || input->KeyHoldDown(moveRigth))
		camera->MoveCamera(-0.2f, 0.0f, 0.0f);
}

void ButtonsActionMap::MoveUp()
{
	if (input->KeyPressedDown(moveUp) || input->KeyHoldDown(moveUp))
		camera->MoveCamera(0.0f, 0.0f, 0.2f);
}

void ButtonsActionMap::MoveDown()
{
	if (input->KeyPressedDown(moveDow) || input->KeyHoldDown(moveDow))
		camera->MoveCamera(0.0f, 0.0f, -0.2f);
}

void ButtonsActionMap::Rotate()
{
	if (input->MouseKeyHoldDown(M_RIGTH))
	{
		camera->SetRotation(input->MouseGetMovementX() * 0.1, input->MouseGetMovementY() * -0.1, 0);
	}
}
