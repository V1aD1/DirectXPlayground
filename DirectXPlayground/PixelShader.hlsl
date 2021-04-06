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
	float4 finalColor = {0,0,0,0};

	if (color1.z != color2.z) {
		// no idea why, but setting finalColor to color1 outside of this if statement
		// doesn't work. For some reason, Texture1 and Texture2 are BOTH the same,
		// unless I play around with their alpha values. I have no idea why.
		finalColor = color2;
	}
	else {
		finalColor = color;
	}

	// fade from one texture to another along U axis
	//float4 finalColor = color1 * texCoord.x + color2 * (1 - texCoord.x);

	// draw one texture over the other
	//finalColor = color1*(1 - color2.a) + color2 * color2.a;

	return color * finalColor;
}