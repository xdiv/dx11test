#include "pch.h"
#include "GUI_Parts.h"

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------

Label::Label(ID2D* d2d, wstring text, float4 position, float4 color)
	: GUI_Base(d2d, position, color), m_stateBlock(nullptr), 
	m_textFormat(nullptr), m_textLayout(nullptr)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
	this->m_text = text;
	m_textFormatModel = {L"Segoe UI", DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15.0f,  &LOCALENAME };
	CreateTextFormat();
	CreateTextLayout();
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
}

Label::~Label()
{
	SAFE_RELEASE(m_textLayout);
	SAFE_RELEASE(m_textFormat);
	SAFE_RELEASE(m_stateBlock);
}

void Label::Render()
{
	m_d2d->GetD2DDeviceContext()->DrawTextLayout(D2D1::Point2F(m_position.x, m_position.y), m_textLayout, m_brush);
}

float4 Label::GetSize()
{
	return float4(m_position.x, m_position.y, m_position.z,
		m_position.w > m_textMetrics.height ?  m_position.w : m_textMetrics.height);
}

void Label::SetTextFormat(TextFormatModel textFormatModel)
{
	m_textFormatModel = textFormatModel;
	CreateTextFormat();
	CreateTextLayout();
}

void Label::SetText(wstring string)
{
	m_text = string;
	CreateTextLayout();
}

void Label::CreateTextFormat()
{
	m_d2d->GetDWriteFactory()->CreateTextFormat(
		m_textFormatModel.fontFamilyName.c_str(), 
		nullptr, 
		m_textFormatModel.fontWeight,
		m_textFormatModel.fontStyle,
		m_textFormatModel.fontStretch, 
		m_textFormatModel.fontSize, 
		m_textFormatModel.localeName.c_str(), 
		&m_textFormat);
}

void Label::CreateTextLayout()
{
	m_d2d->GetDWriteFactory()->CreateTextLayout(m_text.c_str(), (UINT32)m_text.length(),
		m_textFormat, m_position.z, m_position.w, &m_textLayout);
	m_textLayout->GetMetrics(&m_textMetrics);
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
