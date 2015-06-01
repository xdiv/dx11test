struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 PShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	return textureColor;
}