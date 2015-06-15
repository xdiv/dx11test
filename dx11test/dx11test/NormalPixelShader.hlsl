Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PSConstBuffer
{
	float3 color;
	float transperency;
	float hasTexture;
	float hasColor;
	float3 buffer;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
	float4 color	: COLOR0;
};

float4 PShader(PixelInputType input) : SV_TARGET
{
	float4 ret;

	if (hasColor > 0)
		ret = float4(input.color.x, input.color.y, input.color.z, input.color.w);
	else if (hasTexture > 0)
		ret = shaderTexture.Sample(SampleType, input.tex);
	else
		ret = (shaderTexture.Sample(SampleType, input.tex) + input.color) / 2;
	//ret = input.color;
	return ret;
}