#include "InputClass.h"
#include "PublicData.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	//lastKeyboardState = 0;
	//keyboardState = 0;
	m_mouse = 0;

	keyState = 0;
	keyState = new UCHAR*[2];
	keyState[0] = new UCHAR[256];
	keyState[1] = new UCHAR[256];
	
	mouseState = new DIMOUSESTATE[2];
}

InputClass::~InputClass()
{
	Shutdown();
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	SAFE_DELETE(keyState[0]);
	SAFE_DELETE(keyState[0]);
	SAFE_DELETE(keyState);
	SAFE_DELETE(mouseState);

	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	//UCHAR keyboardState2[256];

	// Read the keyboard device.
	
	result = m_keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	else
	{
		keyState[OLD_S] = keyState[NEW_S];
		keyState[NEW_S] = keyboardState;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	else
	{
		mouseState[OLD_S] = mouseState[NEW_S];
		mouseState[NEW_S] = m_mouseState;
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0)  { m_mouseX = 0; }
	if (m_mouseY < 0)  { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

/*
void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}*/

bool InputClass::KeyPressedDown(UINT key)
{
	return keyState[NEW_S][key] & DOWN && keyState[OLD_S][key] & UP;
}

bool InputClass::KeyReleased(UINT key)
{
	return keyState[NEW_S][key] & UP && keyState[OLD_S][key] & DOWN;
}

bool InputClass::KeyHoldDown(UINT key)
{
	return keyState[NEW_S][key] & DOWN && keyState[OLD_S][key] & DOWN;
}

bool InputClass::MouseKeyPressedDown(UINT key)
{
	return mouseState[NEW_S].rgbButtons[key] & DOWN && mouseState[OLD_S].rgbButtons[key] & UP;
}

bool InputClass::MouseKeyReleased(UINT key)
{
	return mouseState[NEW_S].rgbButtons[key] & UP && mouseState[OLD_S].rgbButtons[key] & DOWN;
}

bool InputClass::MouseKeyHoldDown(UINT key)
{
	return mouseState[NEW_S].rgbButtons[key] & DOWN &&  mouseState[OLD_S].rgbButtons[key] & DOWN;
}