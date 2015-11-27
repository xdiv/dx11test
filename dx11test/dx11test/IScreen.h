#pragma once
#include "pch.h"

struct Screen
{
public:
	Screen() : width(0), height(0), heightHalf(0), 
		widthHalf(0), aspect(0), aspectInv(0),
		nearClip(0), farClip(0){};
	Screen(UINT w, UINT h) 
		: width(w), height(h), heightHalf(0), 
		widthHalf(0), aspect(0), aspectInv(0),
		nearClip(0), farClip(0)
	{
		SetRez(w, h);
	}

	void SetRez(UINT w, UINT h)
	{
		widthHalf = width * 0.5;
		heightHalf = heightHalf * 0.5;
		aspect = (float)w / (float)h;
		aspectInv = 1 / aspect;
	}

	void SetClip(float nearClip, float farClip)
	{
		this->nearClip = nearClip;
		this->farClip = farClip;
	}

	//dont use setters
	UINT width, height;
	UINT widthHalf, heightHalf;
	float aspect, aspectInv;
	float nearClip, farClip;
};

__interface IScreen
{
	Screen GetScreen();
};