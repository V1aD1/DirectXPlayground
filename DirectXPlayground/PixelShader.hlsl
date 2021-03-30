Texture2D Texture;
SamplerState ss;

cbuffer ConstantBuffer {
	float3 offset;
}

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR, float2 texCoord: TEXCOORD) : SV_TARGET
{
	return color * Texture.Sample(ss, texCoord);
}