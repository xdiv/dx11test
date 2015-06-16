cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION0;
	float2 tex		: TEXCOORD0;
	float3 intsPosition : INSTANCEPOS0;
	float4 color	: COLOR0;
	float3 scale	: POSITION1;
	float3 rotation : POSITION2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
	float4 color	: COLOR0;
};

float4 ComputeQuatW(float3 quat)
{
	float4 q;

	q.xyz = quat;
	float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);

	if (t < 0.0f)
		q.w = 0.0f;
	else
		q.w = -sqrt(t);

	return q;
}

matrix BuildRotationMatrix(float4 q)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	wx = wy = wz = xx = yy = yz = xy = xz = zz = x2 = y2 = z2 = 0;

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

	matrix m;

	m._11 = 1 - (yy + zz);
	m._12 = xy - wz;
	m._13 = xz + wy;
	m._14 = 0;

	m._21 = xy + wz;
	m._22 = 1 - (xx + zz);
	m._23 = yz - wx;
	m._24 = 0;

	m._31 = xz - wy;
	m._32 = yz + wx;
	m._33 = 1 - (xx + yy);
	m._34 = 0;

	m._41 = m._42 = m._43 = 0;
	m._44 = 1;
	return m;
}

PixelInputType VShader(VertexInputType input)
{
	PixelInputType output;
	float4 quat;
	
	input.position.w = 1.0f;

	output.position = input.position;
	output.position.xyz *= input.scale;

	output.position = mul(output.position, BuildRotationMatrix(ComputeQuatW(input.rotation)));

	output.position.xyz += input.intsPosition;
	
	output.position = mul(output.position, worldMatrix);
	output.tex = input.tex;
	
	//output.position.w = 1;

	output.color = input.color;
	return output;
}