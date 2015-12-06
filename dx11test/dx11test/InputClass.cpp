#include "pch.h"
#include "InputClass.h"


InputClass::InputClass(IScreen* screen)
	: m_mouseState_old(nullptr), m_mouseState_new(nullptr),
	m_keyState_old(nullptr), m_keyState_new(nullptr),
	m_screen(nullptr)
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_screen = screen;
	m_cursor = new POINT();

	m_keyState_old = new UCHAR[KEY_COUNT];
	m_keyState_new = new UCHAR[KEY_COUNT];
	m_mouseState_old = new DIMOUSESTATE();
	m_mouseState_new = new DIMOUSESTATE();
}

InputClass::~InputClass()
{
	Shutdown();
}

void InputClass::Initialize()
{
	HRESULT result;

	// Initialize the main direct input interface.
	result = DirectInput8Create(m_screen->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	assert(SUCCEEDED(result));

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr);
	assert(SUCCEEDED(result));

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(m_screen->GetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	assert(SUCCEEDED(result));

	// Now acquire the keyboard.
	// Gali failinti, ne kritinis;
	result = m_keyboard->Acquire();

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr);
	assert(SUCCEEDED(result));

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(m_screen->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// Acquire the mouse.
	// gali failinti, ne kritinis;
	result = m_mouse->Acquire();
}

void InputClass::Shutdown()
{
	SAFE_DELETE(m_keyState_old);
	SAFE_DELETE(m_keyState_new);

	SAFE_DELETE(m_mouseState_old);
	SAFE_DELETE(m_mouseState_new);

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

void InputClass::Update()
{
	// Read the current state of the keyboard.
	ReadKeyboard();

	// Read the current state of the mouse.
	ReadMouse();

	GetPhysicalCursorPos(m_cursor);
	ScreenToClient(m_screen->GetHWND(), m_cursor);
}

void InputClass::ReadKeyboard()
{
	HRESULT result;
	UCHAR * keyboardState = new UCHAR[KEY_COUNT];

	int x = sizeof(UCHAR) * KEY_COUNT;

	result = m_keyboard->GetDeviceState(x, (LPVOID)keyboardState);

	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
	}
	else
	{
		delete[] m_keyState_old;
		m_keyState_old = m_keyState_new;
		m_keyState_new = keyboardState;
	}
}

void InputClass::ReadMouse()
{
	HRESULT result;

	DIMOUSESTATE *mouseState = new DIMOUSESTATE();
	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
	}
	else
	{
		delete[] m_mouseState_old;
		m_mouseState_old = m_mouseState_new;	
		m_mouseState_new = mouseState;
	}
}

bool InputClass::KeyPressedDown(UINT key)
{
	return m_keyState_new[key] & DOWN && m_keyState_old[key] == UP;
}

bool InputClass::KeyReleased(UINT key)
{
	return m_keyState_new[key] == UP && m_keyState_old[key] & DOWN;
}

bool InputClass::KeyHoldDown(UINT key)
{
	return m_keyState_new[key] & DOWN && m_keyState_old[key] & DOWN;
}

bool InputClass::MouseKeyPressedDown(UINT key)
{
	return m_mouseState_new->rgbButtons[key] & DOWN && m_mouseState_old->rgbButtons[key] == UP;
}

bool InputClass::MouseKeyReleased(UINT key)
{
	return m_mouseState_new->rgbButtons[key] == UP && m_mouseState_old->rgbButtons[key] == DOWN;
}

bool InputClass::MouseKeyHoldDown(UINT key)
{
	return m_mouseState_new->rgbButtons[key] & DOWN &&  m_mouseState_old->rgbButtons[key] & DOWN;
}

LONG InputClass::MouseGetMovementX()
{
	return m_mouseState_new->lX;
}

LONG InputClass::MouseGetMovementY()
{
	return m_mouseState_new->lY;
}