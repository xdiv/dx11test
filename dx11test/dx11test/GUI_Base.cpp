#include "GUI_Base.h"

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
GUI_Base::GUI_Base(rect pos, int margin, float4 color)
{
	this->position = pos;
	this->margin = margin;
	this->color = color;
}

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
Label::Label(string text, rect position, int margin, float4 color)
	: GUI_Base(position, margin, color)
{
	this->text = text;
}

//-------------------------------------------------------------------------------
//			Frame
//-------------------------------------------------------------------------------
Frame::Frame()
{
	parent = 0;
}

Frame::~Frame()
{
}

void Frame::OnClick()
{

}
void Frame::OnMouseEnter()
{

}
void Frame::OnMouseLeave()
{

}
void Frame::SetPosition(rect pos)
{

}
void Frame::Border(int size, float4 color)
{

}

//-------------------------------------------------------------------------------
//			Button
//-------------------------------------------------------------------------------
Button::Button()
{
	label = 0;
}

Button::~Button()
{
	SAFE_DELETE(label);
}