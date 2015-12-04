#include "RayCast.h"
#include "pch.h"

//namespace RayCast
//{
	//XMFLOAT3 CameraToWorldRay(Screen screen, POINT location, XMMATRIX rotation)
	//{
	//	float f = tanf(screen.fow);
	//	float dx = f * ((location.x - screen.x) / screen.widthHalf - 1.f) * screen.aspectInv;
	//	float dy = f * (1.f - (location.y - screen.y) / screen.heightHalf) * screen.aspectInv;
	//	float dz = screen.farClip - screen.nearClip;
	//	XMVECTORF32 vec1 = { dx * dz, dy * dz, dz, 0 };
	//	XMVECTORF32 vec2 = { dx, dy, 1, 0 };
	//	XMVECTORF32 vec3;
	//	vec3.v = DirectX::XMVector3Normalize(vec1);
	//	return XMFLOAT3();
	//}
//}
