#pragma once

//#include <crtdefs.h>

#ifndef __BASE_TYPES__
#define __BASE_TYPES__

typedef struct float2
{
	float x, y;
	float2(){};
	float2(float f);
	float2(float fx, float fy);
	const void Zero() { x = y = 0;};

	bool operator!=(float2 a);
	bool operator==(float2 a);
} float2;

typedef struct float3
{
public:

	float x, y, z;

	float3() {};
	float3(float f);
	float3(float x, float y, float z);

	static float3 Zero() { float3(0); };

	// assignment operators
	float3& operator += (const float3&);
	float3& operator -= (const float3&);
	float3& operator *= (float a);
	float3& operator /= (float a);

	// unary operators
	float3 operator + () const;
	float3 operator - () const;

	// binary operators
	float3 operator + (const float3& a) const;
	float3 operator - (const float3& a) const;
	float3 operator * (float a) const;
	float3 operator / (float a) const;

	friend float3 operator * (float a, const float3 &);

	bool operator!=(float3 a);
	bool operator==(float3 a);
} float3, color_rgb;

typedef struct float4
{
public:
	float x, y, z, w;

	float4() {};
	float4(float x, float y, float z, float w);
	const void Zero() { x = y = z = w = 0; };

	// binary operators
	float4 operator * (float a) const;
	float4 operator / (float a) const;

	bool operator!=(float4 a);
	bool operator==(float4 a);
} float4, color_rgba;

typedef struct int3
{
public:
	int x, y, z;

	bool operator!=(int3 a);
	bool operator==(int3 a);
} int3;

typedef struct Joint
{
	int Parent;
	float3 Pos;
	float4 Orient;

	bool operator!=(Joint a);
	bool operator==(Joint a);
} Joint;

typedef struct Vertice
{
	float2 Tex;
	unsigned int StartWeight;
	unsigned int WeightCount;

	bool operator!=(Vertice a);
	bool operator==(Vertice a);
} Vertice;

typedef struct  Weight
{
	unsigned int JointId;
	float Bias;
	float3 Pos;

	bool operator!=(Weight a);
	bool operator==(Weight a);
} Weight;

struct Mesh
{
public:
	unsigned long VerticesCount,
		TrianglesCount, 
		WeightCount;

	Mesh() { Vertices = 0;  
		Triangles = 0;  
		Weights = 0;
	};
	Vertice * Vertices;
	int3 * Triangles;
	Weight * Weights;
};

typedef struct Matrix3x3
{
public:
	float
		_11, _12, _13,
		_21, _22, _23,
		_31, _32, _33;
} Matrix3x3;

typedef struct rect
{
public:
	unsigned int x, y, width, heigth;
	rect(){ x = y = width = heigth = 0; };
} rect;

//--------------------------------------------------------------------------------------------------------------
//meshes
//--------------------------------------------------------------------------------------------------------------
typedef struct meshv1{
	float3   Pos;
	float2   Tex0;
	float3   Normal;
	int           StartWeight;
	int           WeightCount;
} meshv1;

typedef struct mesh2d{
public:
	mesh2d(){};
	mesh2d(float px, float py, float pz, float tx, float ty);
	float3   Pos;
	float2   Tex0;
} mesh2d;

//--------------------------------------------------------------------------------------------------------------
//meshes
//--------------------------------------------------------------------------------------------------------------
typedef struct InstanceType_A
{
public:
	InstanceType_A(){};
	InstanceType_A(float x, float y, float z);
	InstanceType_A(float3 p);
	float3 position;
}InstanceType_A;

#endif