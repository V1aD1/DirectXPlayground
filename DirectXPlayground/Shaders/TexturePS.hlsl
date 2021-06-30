Texture2D<float4> Texture: register(t0);

SamplerState ss: register(s0);

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color : COLOR, float2 texCoord : TEXCOORD) : SV_TARGET
{
	float4 texColor = Texture.Sample(ss, texCoord);
	float4 finalColor = texColor * color;
	return finalColor;
}