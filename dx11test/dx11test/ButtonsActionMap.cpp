#include "pch.h"
#include "ButtonsActionMap.h"

ButtonsActionMap::ButtonsActionMap(InputClass* input)
	: m_input(nullptr)
{
	m_input = input;
	m_moveFoward	= KEY_W;
	m_moveBack		= KEY_S;
	m_moveLeft		= KEY_A;
	m_moveRigth		= KEY_D;
	m_moveUp		= KEY_SPACE;
	m_moveDown		= KEY_C;
	m_exit			= KEY_ESCAPE;
}


ButtonsActionMap::~ButtonsActionMap()
{
	SAFE_DELETE(m_input);
}

void ButtonsActionMap::Update()
{
	MoveEvent();
	CameraRotate();
	Exit();
	FullScreenSwitch();
	MouseButton1Click();
}

void ButtonsActionMap::MoveEvent()
{
	short x = 0, y = 0, z = 0; 

	if (m_input->KeyHoldDown(m_moveFoward))
		y++;
	if (m_input->KeyHoldDown(m_moveBack))
		y--;
	if (m_input->KeyHoldDown(m_moveLeft))
		x++;
	if (m_input->KeyHoldDown(m_moveRigth))
		x--;
	if (m_input->KeyHoldDown(m_moveUp))
		z++;
	if (m_input->KeyHoldDown(m_moveDown))
		z--;

	if( x != 0 || y != 0 || z != 0)
		__raise MoveEvent(1, 2, 3);
}
void ButtonsActionMap::CameraRotate()
{
	if (m_input->MouseKeyHoldDown(M_RIGTH))
	{
		__raise CameraRotate(m_input->MouseGetMovementX(), m_input->MouseGetMovementY());
	}
}
void ButtonsActionMap::Exit()
{
	if ((m_input->KeyHoldDown(KEY_LMENU)
		|| m_input->KeyHoldDown(KEY_RMENU))
		&& m_input->KeyPressedDown(KEY_F4)
		|| m_input->KeyPressedDown(KEY_ESCAPE))
		__raise ExitEvent();
}
void ButtonsActionMap::FullScreenSwitch()
{
	if (m_input->MouseKeyPressedDown(KEY_F10))
		__raise FullScreenSwitchEvent();
}

void ButtonsActionMap::MouseButton1Click()
{
	if (m_input->MouseKeyReleased(M_LEFT))
	{
		__raise MouseButton1ClickEvent();
	}
}

