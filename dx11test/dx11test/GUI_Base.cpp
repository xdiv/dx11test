#include "GUI_Base.h"

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
GUI_Base::GUI_Base(ID2D* d2d)
	: m_d2d(d2d), m_brush(), m_stateBlock()
{
	Init(D2D1::ColorF(D2D1::ColorF::White));
}

GUI_Base::GUI_Base(ID2D* d2d, float4 pos, float4 color)
	: m_d2d(d2d), m_brush(), m_stateBlock()
{
	m_position = pos;
	m_color = color;

	Init(D2D1::ColorF(color.x, color.y, color.z, color.w));
}

GUI_Base::~GUI_Base()
{
	SAFE_RELEASE(m_stateBlock);
	SAFE_RELEASE(m_brush);
}

void GUI_Base::SetPosition(float4 pos)
{
	m_position = pos;
}

void GUI_Base::SetColor(float4 color)
{
	m_color = color;
}

void GUI_Base::Init(D2D1::ColorF color)
{
	hr = m_d2d->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock);
	hr = m_d2d->GetD2DDeviceContext()->CreateSolidColorBrush(color, &m_brush);
}