#pragma once
//#include <windef.h>
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "../cdxml/complex_types.h"
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
