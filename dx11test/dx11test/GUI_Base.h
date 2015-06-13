#pragma once
//#include <windef.h>
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "complex_types.h"
#include "PublicData.h"

using namespace std;
//-------------------------------------------------------------------------------
//			Enums
//-------------------------------------------------------------------------------
enum Aligment
{
	left	= 0x0,
	top		= 0x0,
	hCenter = 0x1,
	rigth	= 0x2,
	buttom  = 0x4,
	vCenter	= 0x8
};

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
class GUI_Base
{
protected:
	rect position; // absolute position
	int margin;
	float4 color;
	Aligment	hAligment;

private:
	GUI_Base(GUI_Base&) {};
public:
	GUI_Base() {};
	GUI_Base(rect, int, float4);
	~GUI_Base() {};
};

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
class Label : GUI_Base
{
protected:
	string text;
private:
	Label(Label&);
public:
	Label(string text, rect position, int margin, float4 color);
	~Label() {};

	void Render();
};

//-------------------------------------------------------------------------------
//			Frame
//-------------------------------------------------------------------------------
class Frame : GUI_Base
{
protected:
	int paddingSize;
	int borderSize;
	float4 borderColor;
	Frame* parent;
	vector<Frame> childList;
public:
	Frame();
	~Frame();

	void Render();

	void OnClick();
	void OnMouseEnter();
	void OnMouseLeave();
	void SetPosition(rect pos);
	void Border(int size, float4 color);
};

//-------------------------------------------------------------------------------
//			Button
//-------------------------------------------------------------------------------
class Button : Frame
{
protected:
	Label* label;
public:
	Button();
	~Button();
};
