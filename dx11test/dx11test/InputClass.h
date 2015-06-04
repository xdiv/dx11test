#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputClass
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	UCHAR m_keyboardState[256];
	UCHAR lastKeyboardState[256];
	DIMOUSESTATE m_mouseState;
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

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool MoveLeft();
	bool MoveRight();
	bool MoveUp();
	bool MoveDown();
	void GetMouseLocation(int&, int&);
};

