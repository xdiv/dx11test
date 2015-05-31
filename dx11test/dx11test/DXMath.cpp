
#include <math.h>
#include "DXMath.h"


//void  ComputeQuatW(D3DXVECTOR4& quat)//D3DXVECTOR
//{
//	float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
//	if (t < 0.0f)
//	{
//		quat.w = 0.0f;
//	}
//	else
//	{
//		quat.w = -sqrtf(t);
//	}
//}
//
//void  ComputeQuatW(D3DXQUATERNION& quat)//D3DXVECTOR
//{
//	float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
//	if (t < 0.0f)
//	{
//		quat.w = 0.0f;
//	}
//	else
//	{
//		quat.w = -sqrtf(t);
//	}
//}
//
//void lerp(D3DXVECTOR4 *rez, D3DXVECTOR4 &a, D3DXVECTOR4 &b, float &t)
//{
//	rez = new D3DXVECTOR4((1 - t)*a + t*b);
//}
//
//Matrix3x3 BuildRotationMatrix(D3DXQUATERNION& q)
//{
//	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
//
//	x2 = q.x + q.x; 
//	y2 = q.y + q.y;
//	z2 = q.z + q.z;
//	xx = q.x * x2; 
//	xy = q.x * y2; 
//	xz = q.x * z2;
//	yy = q.y * y2; 
//	yz = q.y * z2;
//	zz = q.z * z2;
//	wx = q.w * x2; 
//	wy = q.w * y2; 
//	wz = q.w * z2;
//
//	Matrix3x3 m;
//
//	m._11 = 1 - (yy + zz);
//	m._12 = xy - wz;
//	m._13 = xz + wy;
//
//	m._21 = xy + wz;
//	m._22 = 1 - (xx + zz);
//	m._23 = yz - wx;
//
//	m._31 = xz - wy;
//	m._32 = yz + wx;
//	m._33 = 1 - (xx + yy);
//	return m;
//}
//
//Matrix3x3 BuildRotationMatrix(D3DXVECTOR4& quat)
//{
//	return BuildRotationMatrix(D3DXQUATERNION(quat.x, quat.y, quat.z, quat.w));
//}
//
//D3DXVECTOR3 Multiply(D3DXQUATERNION& q, D3DXVECTOR3& v)
//{
//	D3DXVECTOR3 vr;
//	Matrix3x3 m = BuildRotationMatrix(q);
//	vr.x = m._11 * v.x + m._12 *v.y + m._13 * v.z;
//	vr.y = m._21 * v.x + m._22 *v.y + m._23 * v.z;
//	vr.z = m._31 * v.x + m._32 *v.y + m._33 * v.z;
//	return vr;
//}
//
//D3DXVECTOR3 Multiply(D3DXVECTOR4& q, D3DXVECTOR3& v)
//{
//	D3DXVECTOR3 vr;
//	Matrix3x3 m = BuildRotationMatrix(q);
//	vr.x = m._11 * v.x + m._12 *v.y + m._13 * v.z;
//	vr.y = m._21 * v.x + m._22 *v.y + m._23 * v.z;
//	vr.z = m._31 * v.x + m._32 *v.y + m._33 * v.z;
//	return vr;
//}
//
//D3DXVECTOR3 Vec3Cross(D3DXVECTOR3& a, D3DXVECTOR3& b)
//{
//	D3DXVECTOR3 v;
//
//	v.x = a.y * b.z - a.z * b.y;
//	v.y = a.z * b.x - a.x * b.z;
//	v.z = a.x * b.y - a.y * b.x;
//
//	return v;
//}
//
//D3DXVECTOR3 CalcNormals(D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c)
//{
//	D3DXVECTOR3 r;
//	r = Vec3Cross(b - a, c - a);
//	r = Normalize(r);
//	return r;
//}
//
//D3DXVECTOR3 Normalize(D3DXVECTOR3& v)
//{
//	return v / Length(v);
//}
//
//float Length(D3DXVECTOR3& v)
//{
//	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
//}
//
//float Length(D3DXQUATERNION& v)
//{
//	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w* v.w);
//}
//
//D3DXQUATERNION Normalize(D3DXQUATERNION v)
//{
//	return v / Length(v);
//}


// float3


float3 Multiply(float4& q, float3& v)
{
	Matrix3x3 m = BuildRotationMatrix(q);
	return float3(m._11 * v.x + m._12 *v.y + m._13 * v.z,
		m._21 * v.x + m._22 *v.y + m._23 * v.z,
		m._31 * v.x + m._32 *v.y + m._33 * v.z);
}


float3 CalcNormals(float3& a, float3& b, float3& c)
{
	float3 r;
	r = Vec3Cross(b - a, c - a);
	r = Normalize(r);
	return r;
}

float3 Vec3Cross(float3& a, float3& b)
{
	return float3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

float3 Normalize(float3& v)
{
	return v / Length(v);
}

float Length(float3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

//float4
void  ComputeQuatW(float4& quat)//D3DXVECTOR
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

Matrix3x3 BuildRotationMatrix(float4& q)
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

//void lerp(float4 *rez, float4 &a, float4 &b, float &t)
//{
//
//}
//
//float Length(float4&)
//{
//
//}
//
//float4 Normalize(float4& v)
//{
//	return v / Length(v);
//}