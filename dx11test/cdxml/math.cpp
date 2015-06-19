#include "math.h"
#include "stdafx.h"
#include <math.h>

CDXML_API  float3 Multiply(float4& q, float3& v)
{
	Matrix3x3 m = BuildRotationMatrix(q);
	return float3(m._11 * v.x + m._12 *v.y + m._13 * v.z,
		m._21 * v.x + m._22 *v.y + m._23 * v.z,
		m._31 * v.x + m._32 *v.y + m._33 * v.z);
}

CDXML_API  float3 CalcNormals(float3& a, float3& b, float3& c)
{
	float3 r;
	r = Vec3Cross(b - a, c - a);
	r = Normalize(r);
	return r;
}

CDXML_API  float3 Vec3Cross(float3& a, float3& b)
{
	return float3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

CDXML_API  float3 Normalize(float3& v)
{
	return v / Length(v);
}

CDXML_API  float Length(float3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

//float4
CDXML_API void  ComputeQuatW(float4& quat)//D3DXVECTOR
{
	float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
	if (t < 0.0f)
	{
		quat.w = 0.0f;
	}
	else
	{
		quat.w = -sqrtf(t);
	}
}

CDXML_API Matrix3x3 BuildRotationMatrix(float4& q)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = q.x + q.x;
	y2 = q.y + q.y;
	z2 = q.z + q.z;
	xx = q.x * x2;
	xy = q.x * y2;
	xz = q.x * z2;
	yy = q.y * y2;
	yz = q.y * z2;
	zz = q.z * z2;
	wx = q.w * x2;
	wy = q.w * y2;
	wz = q.w * z2;

	Matrix3x3 m;

	m._11 = 1 - (yy + zz);
	m._12 = xy - wz;
	m._13 = xz + wy;

	m._21 = xy + wz;
	m._22 = 1 - (xx + zz);
	m._23 = yz - wx;

	m._31 = xz - wy;
	m._32 = yz + wx;
	m._33 = 1 - (xx + yy);
	return m;
}