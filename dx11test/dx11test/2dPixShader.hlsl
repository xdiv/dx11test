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
	/*if (hasColor && !hasTexture)
	{
		return float4(color.xyz, tranceperency);
	}
	else if (!hasColor && hasTexture)
	{
		float4 textureColor;
		textureColor = shaderTexture.Sample(SampleType, input.tex);
		textureColor.w = tranceperency;
		return textureColor;
	}
	else
	{
		float4 textureColor;
		textureColor = shaderTexture.Sample(SampleType, input.tex);
		return float4((textureColor.xyz + color) / 2, tranceperency);
	}*/
	//ret = float4(1.0f, 0.0f, 0.0f, 0.3f);
	//return float4(0.0f, 1.0f, 0.0f, 0.3f);
	return ret;
}