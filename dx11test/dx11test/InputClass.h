#pragma once

#ifndef __INPUTCLASS__

#define __INPUTCLASS__
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "pch.h"
#include "GameWindow.h"

#define KEY_COUNT 256 // istikro tai yra 237

const UINT DOWN = 0x80;
const UINT UP = 0x00;

class InputClass
{
public:
	InputClass(IScreen* screen);
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();
	void Shutdown();
	void Update();

	bool KeyPressedDown(UINT key);
	bool KeyReleased(UINT key);
	bool KeyHoldDown(UINT key);

	bool MouseKeyPressedDown(UINT key);
	bool MouseKeyReleased(UINT key);
	bool MouseKeyHoldDown(UINT key);

	LONG MouseGetMovementX();
	LONG MouseGetMovementY();

	LPPOINT GetMouseLocation() { return m_cursor; };

private:
	IDirectInput8*			m_directInput;
	IDirectInputDevice8*	m_keyboard;
	IDirectInputDevice8*	m_mouse;

	IScreen*		m_screen;

	UCHAR*			m_keyState_old;
	UCHAR*			m_keyState_new;
	DIMOUSESTATE*	m_mouseState_old;
	DIMOUSESTATE*	m_mouseState_new;

	LPPOINT			m_cursor;

	void ReadKeyboard();
	void ReadMouse();
};
#endif
