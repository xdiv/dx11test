struct VertexInputType
{
	float4 position : POSITION;
	float2 tex		: TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};

PixelInputType VShader(VertexInputType input)
{
	PixelInputType ret;
	ret.position = input.position;

	ret.position.z = 0.0f;
	ret.position.w = 1.0f;

	ret.tex = input.tex;
	return ret;
}