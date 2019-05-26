Texture2D in_texture;
SamplerState state;



struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	float4 texturized = in_texture.Sample(state, input.TexCoord);
	float4 uv_color = float4(input.TexCoord.x, input.TexCoord.y, input.TexCoord.x * input.TexCoord.y, 1);
	return texturized;
}

float4 simple_ps(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	float4 texturized = in_texture.Sample(state, input.TexCoord);
	return color;
}