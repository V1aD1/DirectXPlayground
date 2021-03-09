cbuffer ConstantBuffer {
	float3 offset;
}

// order of input variables MUST MATCH order of return values from vertex shader
float4 main(float4 position: SV_POSITION, float4 color: COLOR) : SV_TARGET
{
	//color *= offset.x;
	return color;
}