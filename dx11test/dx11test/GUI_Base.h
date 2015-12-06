#pragma once
//#include <windef.h>
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "../cdxml/complex_types.h"
#include "pch.h"
#include "DirectX11.h"
#include "TypeHelper.h"


using namespace std;

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
class GUI_Base
{
public:
	GUI_Base(ID2D* d2d);
	GUI_Base(ID2D* d2d, Rect_F poz, D2D1_COLOR_F color);
	~GUI_Base();

	void SetPosition(Rect_F pos);
	void SetColor(D2D1_COLOR_F color);
	void SetTransperancy(float t);

	ID2D1SolidColorBrush* GetBrush() { return m_brush; }

	virtual void Render() = 0;
	virtual Rect_F GetSize() = 0;

protected:
	Rect_F			m_position; // absolute m_position
	D2D1_COLOR_F	m_color;

	//shared
	ID2D* m_d2d;

	//private
	ID2D1SolidColorBrush*	m_brush;
	ID2D1DrawingStateBlock* m_stateBlock;

	void Init(D2D1_COLOR_F color);
	GUI_Base(GUI_Base&) {};
};
