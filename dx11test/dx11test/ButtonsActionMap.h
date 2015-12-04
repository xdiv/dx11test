#pragma once

#ifndef __ACTION_MAP__
#define __ACTION_MAP__

#include <dinput.h>
#include "InputClass.h"
#include "Camera.h"

#pragma region Key codes
//from - dinput.h
const UINT KEY_ESCAPE		= 0x01;//	1	
const UINT KEY_1			= 0x02;//	2	
const UINT KEY_2			= 0x03;//	3	
const UINT KEY_3			= 0x04;//	4	
const UINT KEY_4			= 0x05;//	5	
const UINT KEY_5			= 0x06;//	6	
const UINT KEY_6			= 0x07;//	7	
const UINT KEY_7			= 0x08;//	8	
const UINT KEY_8			= 0x09;//	9	
const UINT KEY_9			= 0x0A;//	10	
const UINT KEY_0			= 0x0B;//	11	
const UINT KEY_MINUS		= 0x0C;//	12	/* - on main keyboard */
const UINT KEY_EQUALS		= 0x0D;//	13	
const UINT KEY_BACK			= 0x0E;//	14	 /* backspace */
const UINT KEY_TAB			= 0x0F;//	15	
const UINT KEY_Q			= 0x10;//	16	
const UINT KEY_W			= 0x11;//	17	
const UINT KEY_E			= 0x12;//	18	
const UINT KEY_R			= 0x13;//	19	
const UINT KEY_T			= 0x14;//	20	
const UINT KEY_Y			= 0x15;//	21	
const UINT KEY_U			= 0x16;//	22	
const UINT KEY_I			= 0x17;//	23	
const UINT KEY_O			= 0x18;//	24	
const UINT KEY_P			= 0x19;//	25	
const UINT KEY_LBRACKET		= 0x1A;//	26	
const UINT KEY_RBRACKET		= 0x1B;//	27	
const UINT KEY_RETURN		= 0x1C;//	28	/* Enter on main keyboard */
const UINT KEY_LCONTROL		= 0x1D;//	29	
const UINT KEY_A			= 0x1E;//	30	
const UINT KEY_S			= 0x1F;//	31	
const UINT KEY_D			= 0x20;//	32	
const UINT KEY_F			= 0x21;//	33	
const UINT KEY_G			= 0x22;//	34	
const UINT KEY_H			= 0x23;//	35	
const UINT KEY_J			= 0x24;//	36	
const UINT KEY_K			= 0x25;//	37	
const UINT KEY_L			= 0x26;//	38	
const UINT KEY_SEMICOLON	= 0x27;//	39	
const UINT KEY_APOSTROPHE	= 0x28;//	40	
const UINT KEY_GRAVE		= 0x29;//	41	 /* accent grave */
const UINT KEY_LSHIFT		= 0x2A;//	42	
const UINT KEY_BACKSLASH	= 0x2B;//	43	
const UINT KEY_Z			= 0x2C;//	44	
const UINT KEY_X			= 0x2D;//	45	
const UINT KEY_C			= 0x2E;//	46	
const UINT KEY_V			= 0x2F;//	47	
const UINT KEY_B			= 0x30;//	48	
const UINT KEY_N			= 0x31;//	49	
const UINT KEY_M			= 0x32;//	50	
const UINT KEY_COMMA		= 0x33;//	51	
const UINT KEY_PERIOD		= 0x34;//	52	/* . on main keyboard */
const UINT KEY_SLASH		= 0x35;//	53	* / on main keyboard */
const UINT KEY_RSHIFT		= 0x36;//	54	
const UINT KEY_MULTIPLY		= 0x37;//	55	 /* * on numeric keypad */
const UINT KEY_LMENU		= 0x38;//	56	 /* left Alt */
const UINT KEY_SPACE		= 0x39;//	57	
const UINT KEY_CAPITAL		= 0x3A;//	58	
const UINT KEY_F1			= 0x3B;//	59	
const UINT KEY_F2			= 0x3C;//	60	
const UINT KEY_F3			= 0x3D;//	61	
const UINT KEY_F4			= 0x3E;//	62	
const UINT KEY_F5			= 0x3F;//	63	
const UINT KEY_F6			= 0x40;//	64	
const UINT KEY_F7			= 0x41;//	65	
const UINT KEY_F8			= 0x42;//	66	
const UINT KEY_F9			= 0x43;//	67	
const UINT KEY_F10			= 0x44;//	68	
const UINT KEY_NUMLOCK		= 0x45;//	69	
const UINT KEY_SCROLL		= 0x46;//	70	 /* Scroll Lock */
const UINT KEY_NUMPAD7		= 0x47;//	71	
const UINT KEY_NUMPAD8		= 0x48;//	72	
const UINT KEY_NUMPAD9		= 0x49;//	73	
const UINT KEY_SUBTRACT		= 0x4A;//	74	/* - on numeric keypad */
const UINT KEY_NUMPAD4		= 0x4B;//	75	
const UINT KEY_NUMPAD5		= 0x4C;//	76	
const UINT KEY_NUMPAD6		= 0x4D;//	77	
const UINT KEY_ADD			= 0x4E;//	78	 /* + on numeric keypad */
const UINT KEY_NUMPAD1		= 0x4F;//	79	
const UINT KEY_NUMPAD2		= 0x50;//	80	
const UINT KEY_NUMPAD3		= 0x51;//	81	
const UINT KEY_NUMPAD0		= 0x52;//	82	
const UINT KEY_DECIMAL		= 0x53;//	83	 /* . on numeric keypad */
const UINT KEY_OEM_102		= 0x56;//	84	 /* <> or \| on RT 102-key keyboard (Non-U.S.) */
const UINT KEY_F11			= 0x57;//	85	
const UINT KEY_F12			= 0x58;//	86	
const UINT KEY_F13			= 0x64;//	87	 /*                     (NEC PC98) */
const UINT KEY_F14			= 0x65;//	88	 /*                     (NEC PC98) */
const UINT KEY_F15			= 0x66;//	89	 /*                     (NEC PC98) */
const UINT KEY_KANA			= 0x70;//	90	 /* (Japanese keyboard)            */
const UINT KEY_ABNT_C1		= 0x73;//	91	 /* /? on Brazilian keyboard */
const UINT KEY_CONVERT		= 0x79;//	92	 /* (Japanese keyboard)            */
const UINT KEY_NOCONVERT	= 0x7B;//	93	 /* (Japanese keyboard)            */
const UINT KEY_YEN			= 0x7D;//	94	 /* (Japanese keyboard)            */
const UINT KEY_ABNT_C2		= 0x7E;//	95	 /* Numpad . on Brazilian keyboard */
const UINT KEY_NUMPADEQUALS = 0x8D;//	96	 /* = on numeric keypad (NEC PC98) */
const UINT KEY_PREVTRACK	= 0x90;//	97	 /* Previous Track (const UINT KEY_CIRCUMFLEX on Japanese ke
const UINT KEY_AT			= 0x91;//	98	 /*                     (NEC PC98) */
const UINT KEY_COLON		= 0x92;//	99	 /*                     (NEC PC98) */
const UINT KEY_UNDERLINE	= 0x93;//	100	 /*                     (NEC PC98) */
const UINT KEY_KANJI		= 0x94;//	101	 /* (Japanese keyboard)            */
const UINT KEY_STOP			= 0x95;//	102	 /*                     (NEC PC98) */
const UINT KEY_AX			= 0x96;//	103	 /*                     (Japan AX) */
const UINT KEY_UNLABELED	= 0x97;//	104	 /*                        (J3100) */
const UINT KEY_NEXTTRACK	= 0x99;//	105	 /* Next Track */
const UINT KEY_NUMPADENTER	= 0x9C;//	106	 /* Enter on numeric keypad */
const UINT KEY_RCONTROL		= 0x9D;//	107	
const UINT KEY_MUTE			= 0xA0;//	108	 /* Mute */
const UINT KEY_CALCULATOR	= 0xA1;//	109	 /* Calculator */
const UINT KEY_PLAYPAUSE	= 0xA2;//	110	 /* Play / Pause */
const UINT KEY_MEDIASTOP	= 0xA4;//	111	 /* Media Stop */
const UINT KEY_VOLUMEDOWN	= 0xAE;//	112	 /* Volume - */
const UINT KEY_VOLUMEUP		= 0xB0;//	113	 /* Volume + */
const UINT KEY_WEBHOME		= 0xB2;//	114	 /* Web home */
const UINT KEY_NUMPADCOMMA	= 0xB3;//	115	 /* , on numeric keypad (NEC PC98) */
const UINT KEY_DIVIDE		= 0xB5;//	116	 /* / on numeric keypad */
const UINT KEY_SYSRQ		= 0xB7;//	117	
const UINT KEY_RMENU		= 0xB8;//	118	 /* right Alt */
const UINT KEY_PAUSE		= 0xC5;//	119	 /* Pause */
const UINT KEY_HOME			= 0xC7;//	120	 /* Home on arrow keypad */
const UINT KEY_UP			= 0xC8;//	121	 /* UpArrow on arrow keypad */
const UINT KEY_PRIOR		= 0xC9;//	122	 /* PgUp on arrow keypad */
const UINT KEY_LEFT			= 0xCB;//	123	 /* LeftArrow on arrow keypad */
const UINT KEY_RIGHT		= 0xCD;//	124	 /* RightArrow on arrow keypad */
const UINT KEY_END			= 0xCF;//	125	 /* End on arrow keypad */
const UINT KEY_DOWN			= 0xD0;//	126	 /* DownArrow on arrow keypad */
const UINT KEY_NEXT			= 0xD1;//	127	 /* PgDn on arrow keypad */
const UINT KEY_INSERT		= 0xD2;//	128	 /* Insert on arrow keypad */
const UINT KEY_DELETE		= 0xD3;//	129	 /* Delete on arrow keypad */
const UINT KEY_LWIN			= 0xDB;//	130	 /* Left Windows key */
const UINT KEY_RWIN			= 0xDC;//	131	 /* Right Windows key */
#pragma endregion

#pragma region Mouse codes
const UINT M_LEFT = 0x00; //0
const UINT M_RIGTH = 0x01;//1
const UINT M_MIDLE = 0x02;//2
#pragma endregion

class ButtonsActionMap
{

private:
	UINT moveForw;
	UINT moveBack;
	UINT moveLeft;
	UINT moveRigth;
	UINT moveUp;
	UINT moveDow;
	UINT exit;
	UINT mouseSelect;

	Camera* camera;
	InputClass *input;

private:
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Rotate();
	
public:
	ButtonsActionMap(HINSTANCE, HWND);
	~ButtonsActionMap();
	void Update();
	void SetCamera(Camera* cam);

	template<typename TUpdate>
	void MouseCick(const TUpdate& update)
	{
		if (input->MouseKeyHoldDown(mouseSelect))
		{
			update();
		}
	}
};

#endif
