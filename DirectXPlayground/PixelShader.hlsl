cbuffer ConstantBuffer {
	float3 offset;
}

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR) : SV_TARGET
{
	float4 outputColor = color;
	outputColor.a = 0.5f;
	return outputColor;
}