cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInput
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};

PixelInput VShader(VertexInput input)
{
	PixelInput output;
	output.position.w = float4(input.position.xyz, 1.f);
	output.tex = input.tex;
	output.position = mul(worldMatrix, output.position);
	return input;
}