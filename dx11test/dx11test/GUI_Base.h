#pragma once
//#include <windef.h>
#include <string>
#include <cstring>
#include <vector>
#include <assert.h>
#include "../cdxml/complex_types.h"
#include "DX_Global.h"
#include "DirectX11.h"


using namespace std;

//-------------------------------------------------------------------------------
//			Gui Base
//-------------------------------------------------------------------------------
class GUI_Base
{
public:
	GUI_Base(ID2D* d2d);
	GUI_Base(ID2D* d2d, float4 poz, float4 color);
	~GUI_Base();
	void SetPosition(float4 pos);
	void SetColor(float4 color);

protected:
	HRESULT		hr;
	float4		m_position; // absolute m_position
	float4		m_color;

	//shared
	ID2D* m_d2d;

	//private
	ID2D1SolidColorBrush*	m_brush;
	ID2D1DrawingStateBlock* m_stateBlock;

protected:
	void Init(D2D1::ColorF color);
	GUI_Base(GUI_Base&) {};
	virtual void UpdatePosition() = 0;
	virtual void Render() = 0;
};
