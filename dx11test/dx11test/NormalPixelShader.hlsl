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
};

float4 PShader(PixelInputType input) : SV_TARGET
{
	float4 ret;
	ret = float4(color.x, color.y, color.z, transperency);
	return ret;
}