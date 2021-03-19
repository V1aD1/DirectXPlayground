cbuffer ConstantBuffer{
	float4x4 final;
	float4x4 rotation;
	float4 lightVec; // diffuse light vector
	float4 lightCol; // diffuse light color
	float4 ambientCol; // ambient light color
}

struct VOut {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VOut main(float4 pos : POSITION, float4 normal: NORMAL)
{
	VOut output;
	output.position = mul(final, pos); // mul is an intrinsic that also handles matrix multiplication
	output.color = ambientCol; // set the color of the vertex to the ambient color

	// finding out the normal in world space of the vertex
	float4 norm = normalize(mul(rotation, normal));

	float diffuseBrightness = saturate(dot(norm, lightVec)); // saturate clamps the value to be between 0 and 1

	// final color
	output.color += lightCol* diffuseBrightness; // diffuse color is ADDED to the color!

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}