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

struct TextFormatModel
{
	wstring				fontFamilyName;
	DWRITE_FONT_WEIGHT	fontWeight;
	DWRITE_FONT_STYLE	fontStyle;
	DWRITE_FONT_STRETCH fontStretch;
	FLOAT				fontSize;
	wstring				localeName;
};

//-------------------------------------------------------------------------------
//			Label
//-------------------------------------------------------------------------------
class Label : GUI_Base
{
public:
	Label(ID2D* d2d, wstring text, 
		Rect_F position = {0}, D2D1_COLOR_F color = { 0 });
	~Label();

	void Render();
	Rect_F GetSize();

	TextFormatModel GetTextFormat()									{ return m_textFormatModel; };

	void			SetTextFormat(TextFormatModel textFormatModel);
	void			SetText(wstring string);
	//void SetTextFormat(TextFormatModel* textFormatModel);

protected:
	IDWriteTextFormat*		m_textFormat;
	ID2D1DrawingStateBlock* m_stateBlock;
	IDWriteTextLayout*      m_textLayout;
	DWRITE_TEXT_METRICS	    m_textMetrics;

	TextFormatModel		m_textFormatModel;

	wstring	m_text;
	bool	m_isDinamic;

	void CreateTextFormat();
	void CreateTextLayout();
private:
	//Label(Label& label) : GUI_Base((GUI_Base)label) {};

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
	GUI_Base* parent;
	vector<GUI_Base> childList;
public:
	Frame(ID2D* d2d, Rect_F position = { 0 }, D2D1_COLOR_F color = { 0 });
	~Frame();

	void Render();

	void OnClick();
	void OnMouseEnter();
	void OnMouseLeave();
	void SetPosition(rect pos);
	void Border(int size, D2D1_COLOR_F color);
};

//-------------------------------------------------------------------------------
//			Button
//-------------------------------------------------------------------------------
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