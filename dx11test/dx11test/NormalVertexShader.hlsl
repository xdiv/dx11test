cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex		: TEXCOORD0;
	float3 intsPosition : INSTANCEPOS0;
	float4 color	: COLOR0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
	float4 color	: COLOR0;
};

PixelInputType VShader(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;
	
	output.position = input.position;
	output.position.xyz += input.intsPosition;
	
	output.position = mul(output.position, worldMatrix);
	output.tex = input.tex;
	
	//output.position.w = 1;

	output.color = input.color;
	return output;
}