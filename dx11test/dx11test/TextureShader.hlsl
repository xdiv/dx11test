cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
	int StartWeight : SW;
	int WeightCount : WC;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
};

Texture2D shaderTexture;
SamplerState SampleType;

PixelInputType VShader(VertexInputType input)
{
	PixelInputType output;
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);

	output.tex = input.tex;

	return output;
}


float4 PShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	return textureColor;
}