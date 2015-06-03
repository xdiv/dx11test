cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	int StartWeight : TEXCOORD1;
	int WeightCount : TEXCOORD2;
	float4 intsPosition : INSTANCEPOS0;
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
	
	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.tex = input.tex;

	output.position.xyz += input.intsPosition;

	return output;
}