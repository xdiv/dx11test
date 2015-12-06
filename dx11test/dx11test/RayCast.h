#pragma once
#include "IScreen.h"

namespace RayCast
{
	using namespace DirectX;

	inline XMFLOAT3 CameraToWorldRay(Screen screen, POINT* location, XMMATRIX rotation, float sx,  float sy)
	{
		XMMATRIX inv = XMMatrixInverse(nullptr, rotation);
		float f = tanf(screen.fow);
		float dx = f * ( (float)(location->x) / (float)screen.widthHalf - 1.f) / sx;
		float dy = f * (1.f - (float)(location->y) / (float)screen.heightHalf) / sy;
		XMVECTORF32 vec = { dx, dy, 1.f, 0 }; // kuo užpildom 4 nari? 0?1?
		vec.v = XMVector3TransformNormal(vec, inv);
		vec.v = DirectX::XMVector3Normalize(vec);
		return XMFLOAT3(vec);
	}
}

