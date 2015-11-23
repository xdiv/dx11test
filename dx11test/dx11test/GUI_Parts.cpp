#include "GUI_Parts.h"

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------

Label::Label(ID2D* d2d, wstring text, float4 position, float4 color)
	: GUI_Base(d2d, position, color),
	m_fontFamilyName(L"Segoe UI"), m_fontWeight(DWRITE_FONT_WEIGHT_LIGHT),
	m_fontStyle(DWRITE_FONT_STYLE_NORMAL), m_fontStretch(DWRITE_FONT_STRETCH_NORMAL),
	m_fontSize(15.0f), m_stateBlock(nullptr), m_textFormat(nullptr), m_textLayout(nullptr)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
	this->m_text = text;
	CreateTextFormat();
	CreateTextLayout();
}

Label::~Label()
{
	SAFE_RELEASE(m_textFormat);
	SAFE_RELEASE(m_stateBlock);
	SAFE_RELEASE(m_textLayout);
	SAFE_DELETE_ARRAY(m_fontFamilyName);
}

void Label::Render()
{
	HRESULT hr;
	hr = m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_d2d->GetD2DDeviceContext()->DrawTextLayout(D2D1::Point2F(m_position.x, m_position.y), m_textLayout, m_brush);
	//m_d2dDevCon->Draw
	
}

void Label::CreateTextFormat()
{
	
	hr = m_d2d->GetDWriteFactory()->CreateTextFormat(m_fontFamilyName, nullptr, m_fontWeight, m_fontStyle,
		m_fontStretch, m_fontSize, &LOCALENAME, &m_textFormat);
	hr = m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void Label::CreateTextLayout()
{
	hr = m_d2d->GetDWriteFactory()->CreateTextLayout(m_text.c_str(), (UINT32)m_text.length(),
		m_textFormat, m_position.z, m_position.w, &m_textLayout);
	hr = m_textLayout->GetMetrics(&m_textMetrics);
}

void Label::UpdatePosition()
{

}

//-------------------------------------------------------------------------------
//			Frame
//-------------------------------------------------------------------------------
//Frame::Frame()
//{
//	parent = 0;
//}
//
//Frame::~Frame()
//{
//}
//
//void Frame::Render()
//{
//	for (size_t i = 0; i < childList.size(); i++)
//	{
//		childList[i].Render();
//	}
//}
//
//void Frame::OnClick()
//{
//
//}
//void Frame::OnMouseEnter()
//{
//
//}
//void Frame::OnMouseLeave()
//{
//
//}
//void Frame::SetPosition(rect pos)
//{
//
//}
//void Frame::Border(int size, float4 color)
//{
//
//}
//
////-------------------------------------------------------------------------------
////			Button
////-------------------------------------------------------------------------------
//Button::Button()
//{
//	label = 0;
//}
//
//Button::~Button()
//{
//	SAFE_DELETE(label);
//}
