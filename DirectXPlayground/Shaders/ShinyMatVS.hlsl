cbuffer ConstantBuffer {
	float4x4 final;
	float4x4 rotation;
	float4 materialCol;
}

struct VOut {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VOut main(float4 pos : POSITION, float4 normal : NORMAL)
{
	//ViewingDirection = CameraPosition - VertexPosition
	//ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
	//SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower

	VOut output;
	output.position = mul(final, pos); // mul is an intrinsic that also handles matrix multiplication
	output.color = materialCol; // final color

	// finding out the normal in world space of the vertex
	float4 norm = normalize(mul(rotation, normal));

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}