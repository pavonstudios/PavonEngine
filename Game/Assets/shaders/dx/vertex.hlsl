#pragma pack_matrix( row_major )

cbuffer UniformBuffer : register ( b0 ) {
	matrix model;
	matrix view;
	matrix proj;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_INPUT {
	float4 Pos : SV_Position;
	float2 tex_coord : TEXCOORD;
};

VS_OUTPUT vs_tex(VS_INPUT input)
{
	VS_OUTPUT vs_output;
	float4 output = (float4)0;
	output = mul(input.Pos, model);
	output = mul(output, view);
	output = mul(output, proj);

	vs_output.pos = output;
	vs_output.TexCoord = input.tex_coord;

	return vs_output;
}

float4 VS(float4 Pos : SV_Position) : SV_POSITION
{
	float4 output = (float4)0;
	output = mul(Pos, model);
	output = mul(output, view);
	output = mul(output, proj);
	return output;
}