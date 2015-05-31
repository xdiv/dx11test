#pragma once

#include "complex_types.h"

//void  ComputeQuatW(D3DXQUATERNION& quat);
//Matrix3x3 BuildRotationMatrix(D3DXQUATERNION&);
//void ComputeQuatW(D3DXVECTOR4& quat);
//void lerp(D3DXVECTOR4 *rez, D3DXVECTOR4 &a, D3DXVECTOR4 &b, float &t);
//
//Matrix3x3 BuildRotationMatrix(D3DXVECTOR4&);
//D3DXVECTOR3 Multiply(D3DXQUATERNION&, D3DXVECTOR3&);
//D3DXVECTOR3 Multiply(D3DXVECTOR4&, D3DXVECTOR3&);
//D3DXVECTOR3 CalcNormals(D3DXVECTOR3&, D3DXVECTOR3&, D3DXVECTOR3&);
//D3DXVECTOR3 Vec3Cross(D3DXVECTOR3&, D3DXVECTOR3&);


float3 Multiply(float4&, float3&);

// float3
float3 CalcNormals(float3&, float3&, float3&);
float3 Vec3Cross(float3&, float3&);
float3 Normalize(float3&);
float Length(float3&);

//float4
void  ComputeQuatW(float4& quat);
Matrix3x3 BuildRotationMatrix(float4&);
//void lerp(float4 *rez, float4 &a, float4 &b, float &t);
//float Length(float4&);
//float4 Normalize(float4&);

