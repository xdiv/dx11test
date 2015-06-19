#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "../cdxml/complex_types.h"
#include "PublicData.h"
#include "GUI_Base.h"

using namespace std;
//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
class Label : GUI_Base
{
protected:
	string text;
	bool isDinamic;
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


//-------------------------------------------------------------------------------
//			checkbox
//-------------------------------------------------------------------------------

class CheckBox : GUI_Base
{
protected:
	Label* label;
	bool checked;
public:
	CheckBox() {};
	~CheckBox() {};

	void OnClick() {};
	void OnChanged() {};
};