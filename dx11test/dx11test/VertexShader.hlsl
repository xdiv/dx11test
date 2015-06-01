cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal: NORMAL0;
	float3 intsPosition : INSTANCEPOS0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
};

PixelInputType VShader(VertexInputType input)
{
	PixelInputType output;
	input.position.x *= input.intsPosition.x;
	input.position.y *= input.intsPosition.y;
	input.position.z *= input.intsPosition.z;
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);

	output.tex = input.tex;

	return output;
}