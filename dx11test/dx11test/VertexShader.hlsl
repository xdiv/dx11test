cbuffer MatrixBuffer
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float4 intsPosition : INSTANCEPOS0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType VShader(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);

	output.position.xyz += input.intsPosition.xyz;
	/*output.position.y += input.intsPosition.y;
	output.position.z += input.intsPosition.z;*/

	output.tex = input.tex;

	return output;
}