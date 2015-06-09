#include "complex_types.h"

//===========================================================================
//
//    float2
//
//===========================================================================

float2::float2(float f)
{
	x = f;
	y = f;
}

float2::float2(float fx, float fy)
{
	x = fx;
	y = fy;
}

bool float2::operator != (float2 a) {
	if (a.x != x || a.y != y)
		return true;
	else
		return false;
}

bool float2::operator==(float2 a) {
	if (a.x == x && a.y == y)
		return true;
	else
		return false;
}

//===========================================================================
//
//    float3
//
//===========================================================================
float3::float3(float nx)
{
	x = nx;
	y = nx;
	z = nx;
}

float3::float3(float nx, float ny, float nz)
{
	x = nx;
	y = ny;
	z = nz;
}

float3& float3::operator += (const float3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

float3& float3::operator -= (const float3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

float3& float3::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

float3& float3::operator /= (float f)
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	return *this;
}

float3 float3::operator + () const
{
	return *this;
}

float3 float3::operator - () const
{
	return float3(-x, -y, -z);
}

float3 float3::operator+(const float3& a) const
{
	return float3(x + a.x, y + a.y, z + a.z);
}

float3 float3::operator-(const float3& a) const
{
	return float3(x - a.x, y - a.y, z - a.z);
}

float3 float3::operator * (float a) const
{
	return float3 (x * a, y * a, z * a);
}

float3 float3::operator / (float a) const
{
	return float3(x / a, y / a, z / a);
}

float3 operator*(float f, const float3& v)
{
	return float3(f* v.x, f * v.y, f * v.z);
}

bool float3::operator != (float3 a) {
	if (a.x != x || a.y != y || a.z != z)
		return true;
	else
		return false;
}

bool float3::operator==(float3 a) {
	if (a.x == x && a.y == y && a.z == z)
		return true;
	else
		return false;
}

//===========================================================================
//
//    float4
//
//===========================================================================
float4::float4(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

float4 float4::operator / (float a) const
{
	return float4(x / a, y / a, z / a, w / a);
}

float4 float4::operator * (float a) const
{
	return float4(x * a, y * a, z * a, w * a);
}

bool float4::operator != (float4 a) {
	if (a.x != x || a.y != y || a.z != z || a.w)
		return true;
	else
		return false;
}

bool float4::operator == (float4 a) {
	if (a.x == x && a.y == y && a.z == z && a.w)
		return true;
	else
		return false;
}
//===========================================================================
//
//    int3
//
//===========================================================================
bool int3::operator != (int3 a) {
	if (a.x != x || a.y != y || a.z != z)
		return true;
	else
		return false;
}

bool int3::operator == (int3 a) {
	if (a.x == x && a.y == y && a.z == z)
		return true;
	else
		return false;
}

//===========================================================================
//
//    Joint
//
//===========================================================================
bool Joint::operator != (Joint a) {
	if (a.Pos != Pos || a.Orient != Orient || a.Parent != Parent)
		return true;
	else
		return false;
}

bool Joint::operator == (Joint a) {
	if (a.Pos == Pos && a.Orient == Orient && a.Parent == Parent)
		return true;
	else
		return false;
}
//===========================================================================
//
//    Vertice
//
//===========================================================================
bool Vertice::operator != (Vertice a){
	if (a.StartWeight != StartWeight
		|| a.WeightCount != WeightCount
		|| a.Tex != Tex)
		return true;
	else
		return false;
}

bool Vertice::operator == (Vertice a) {
	if (a.StartWeight == StartWeight
		&& a.WeightCount == WeightCount
		&& a.Tex == Tex)
		return true;
	else
		return false;
}
//===========================================================================
//
//    Weight
//
//===========================================================================

bool Weight::operator != (Weight a) {
	if (a.JointId != JointId
		|| a.Bias != Bias
		|| a.Pos != Pos)
		return true;
	else
		return false;
}

bool Weight::operator == (Weight a) {
	if (a.JointId == JointId
		&& a.Bias == Bias
		&& a.Pos == Pos)
		return true;
	else
		return false;
}


//===========================================================================
//
//    InstanceType_A
//
//===========================================================================
InstanceType_A::InstanceType_A(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

InstanceType_A::InstanceType_A(float3 p)
{
	position = p;
}