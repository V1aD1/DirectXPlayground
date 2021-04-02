Texture2D Texture1;
Texture2D Texture2;

SamplerState ss;

cbuffer ConstantBuffer {
	float3 offset;
}

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR, float2 texCoord: TEXCOORD) : SV_TARGET
{
	float4 ambientColor = {1,1,1,1};

	float4 color1 = ambientColor * Texture1.Sample(ss, texCoord);
	float4 color2 = ambientColor * Texture2.Sample(ss, texCoord);
	
	// fade from one texture to another along U axis
	float4 finalColor = color1 * texCoord.x + color2 * (1 - texCoord.x);

	return finalColor;
}