Texture2D Texture1;
Texture2D Texture2;

SamplerState ss;

cbuffer ConstantBuffer {
	float3 offset;
}

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR, float2 texCoord: TEXCOORD) : SV_TARGET
{
	float4 color1 = Texture1.Sample(ss, texCoord);
	float4 color2 = Texture2.Sample(ss, texCoord);
	return color1 * (1 - color2.a) + color2 * color2.a;
}