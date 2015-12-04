#pragma once
#include "pch.h"

struct Screen : public POINT
{
public:
	Screen(UINT w, UINT h) 
		:	width(0),		height(0), 
			heightHalf(0),  widthHalf(0),	
			aspect(0), aspectInv(0),
			nearClip(0),	farClip(0),
			fow((float)DirectX::XM_PIDIV4)
	{
		SetRez(w, h);
	}

	void SetRez(UINT w, UINT h)
	{
		width = w;
		height = h;
		widthHalf = w * 0.5;
		heightHalf = h * 0.5;
		aspect = (float)w / (float)h;
		//aspect = 1.333333f;
		aspectInv = 1 / aspect;
	}

	void SetClip(float nearClip, float farClip)
	{
		this->nearClip = nearClip;
		this->farClip = farClip;
	}

	UINT	width,		height;
	UINT	widthHalf,	heightHalf;
	float	aspect,		aspectInv;
	float	nearClip,	farClip;
	float	fow;
};

struct MouseLocation
{
	UINT width, height;
};

__interface IScreen
{
	Screen		GetScreen();
	bool		GetGameWindowRect(LPRECT lpRect);
	HINSTANCE	GetHinstance();
};