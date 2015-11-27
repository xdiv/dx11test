#include "pch.h"
#include "GUI_Base.h"

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
GUI_Base::GUI_Base(ID2D* d2d)
	: m_d2d(d2d), m_brush(), m_stateBlock()
{
	Init(D2D1::ColorF(D2D1::ColorF::White));
}

GUI_Base::GUI_Base(ID2D* d2d, Rect_F pos, D2D1_COLOR_F color)
	: m_d2d(d2d), m_brush(), m_stateBlock()
{
	m_position = pos;
	m_color = color;

	Init(color);
}

GUI_Base::~GUI_Base()
{
	SAFE_RELEASE(m_stateBlock);
	SAFE_RELEASE(m_brush);
}

void GUI_Base::SetPosition(Rect_F pos)
{
	m_position = pos;
}

void GUI_Base::SetColor(D2D1_COLOR_F color)
{
	m_color = color;
	m_d2d->GetD2DDeviceContext()->CreateSolidColorBrush(color, &m_brush);
}

void GUI_Base::SetTransperancy(float t)
{
	m_color.a = t;
	m_d2d->GetD2DDeviceContext()->CreateSolidColorBrush(m_color, &m_brush);
}

void GUI_Base::Init(D2D1_COLOR_F color)
{
	m_d2d->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock);
	m_d2d->GetD2DDeviceContext()->CreateSolidColorBrush(color, &m_brush);
}