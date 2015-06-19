#include "complex_types.h"

CDXML_API float3 Multiply(float4& q, float3& v);

// float3
CDXML_API float3 CalcNormals(float3&, float3&, float3&);
CDXML_API float3 Vec3Cross(float3&, float3&);
CDXML_API float3 Normalize(float3&);
CDXML_API float Length(float3&);

//float4
CDXML_API void  ComputeQuatW(float4& quat);
CDXML_API Matrix3x3 BuildRotationMatrix(float4&);