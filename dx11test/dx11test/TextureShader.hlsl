cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

	//output.position = input.position;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}


float4 PShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	return textureColor;
}