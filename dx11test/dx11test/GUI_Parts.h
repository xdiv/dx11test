#pragma once

#ifndef __GUI_PARTS__
#define __GUI_PARTS__
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "../cdxml/complex_types.h"
#include "GUI_Base.h"

using namespace std;

const WCHAR LOCALENAME = L'en-US';

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
class Label : GUI_Base
{
public:
	Label(ID2D* d2d, wstring text = L"", 
		float4 position = {0}, float4 color = { 0 });
	~Label();

	void Render();

protected:
	IDWriteTextFormat*		m_textFormat;
	ID2D1DrawingStateBlock* m_stateBlock;
	IDWriteTextLayout*      m_textLayout;
	DWRITE_TEXT_METRICS	    m_textMetrics;

	WCHAR*				m_fontFamilyName;
	DWRITE_FONT_WEIGHT	m_fontWeight;
	DWRITE_FONT_STYLE	m_fontStyle;
	DWRITE_FONT_STRETCH m_fontStretch;
	FLOAT				m_fontSize;

	wstring	m_text;
	bool	m_isDinamic;

	void CreateTextFormat();
	void CreateTextLayout();
	void UpdatePosition();
private:
	//Label(Label& label) : GUI_Base((GUI_Base)label) {};

};

//-------------------------------------------------------------------------------
//			Frame
//-------------------------------------------------------------------------------
//class Frame : GUI_Base
//{
//protected:
//	int paddingSize;
//	int borderSize;
//	float4 borderColor;
//	Frame* parent;
//	vector<Frame> childList;
//public:
//	Frame();
//	~Frame();
//
//	void Render();
//
//	void OnClick();
//	void OnMouseEnter();
//	void OnMouseLeave();
//	void SetPosition(rect pos);
//	void Border(int size, float4 color);
//};
//
////-------------------------------------------------------------------------------
////			Button
////-------------------------------------------------------------------------------
//class Button : Frame
//{
//protected:
//	Label* label;
//public:
//	Button();
//	~Button();
//};
//
//
////-------------------------------------------------------------------------------
////			checkbox
////-------------------------------------------------------------------------------
//
//class CheckBox : GUI_Base
//{
//protected:
//	Label* label;
//	bool checked;
//public:
//	CheckBox() {};
//	~CheckBox() {};
//
//	void OnClick() {};
//	void OnChanged() {};
//};

#endif