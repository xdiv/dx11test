#include "GUI_Parts.h"

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
Label::Label(string text, rect position, int margin, float4 color)
	: GUI_Base(position, margin, color)
{
	this->text = text;
}

void Label::Render()
{

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

void Frame::Render()
{
	for (size_t i = 0; i < childList.size(); i++)
	{
		childList[i].Render();
	}
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
