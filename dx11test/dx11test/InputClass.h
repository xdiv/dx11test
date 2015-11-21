#pragma once

#ifndef __INPUTCLASS__

#define __INPUTCLASS__
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "DX_Global.h"

const UINT DOWN = 0x80;
const UINT UP = 0x15;

const UINT OLD_S = 1;
const UINT NEW_S = 0;

class InputClass
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	UINT buttonCount;
	UCHAR keyboardState[256];
	//UCHAR lastKeyboardState[256];
	UCHAR **keyState;
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE *mouseState;
	DIMOUSESTATE m_mouseLastState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();

	//void GetMouseLocation(int&, int&);

	bool KeyPressedDown(UINT key);
	bool KeyReleased(UINT key);
	bool KeyHoldDown(UINT key);

	bool MouseKeyPressedDown(UINT key);
	bool MouseKeyReleased(UINT key);
	bool MouseKeyHoldDown(UINT key);

	LONG MouseGetMovementX();
	LONG MouseGetMovementY();
};
#endif
