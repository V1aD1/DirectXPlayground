// due to optimizations, Texture1 isn't visible in the HLSL debugger...
// have to pass debug flag D3DCOMPILE_SKIP_OPTIMIZATION when debugging shaders?
Texture2D<float4> Texture1: register(t0);
Texture2D<float4> Texture2: register(t1);

SamplerState ss: register(s0);

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR, float2 texCoord: TEXCOORD) : SV_TARGET
{
	float4 color1 = Texture1.Sample(ss, texCoord);
	float4 color2 = Texture2.Sample(ss, texCoord);
	float4 finalColor = color2;
	
	return finalColor;
}