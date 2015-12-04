/* jei reikia teksturos
shaderis reikalauja kad butu permatomumas ijungtas;
Texture2D shaderTexture : register(t0);;
SamplerState SampleType;
*/
cbuffer PSConstBuffer
{
	float3 color;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};

float4 PShader(PixelInput input) : SV_TARGET
{
	float a = input.tex.x * input.tex.y + input.tex.x * input.tex.y;
	a = sqrt(a);
	if (a > 1)
		a = 2.f - a;
	return float4(color.xyz, a);
}