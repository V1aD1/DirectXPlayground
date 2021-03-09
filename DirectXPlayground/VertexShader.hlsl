cbuffer ConstantBuffer{
	float4x4 matFinal;
}

struct VOut {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VOut main(float4 pos : POSITION, float4 color: COLOR)
{
	VOut output;
	output.position = mul(matFinal, pos); // mul is an intrinsic that also handles matrix multiplication
	output.color = color;

	// modify the location of each vertex
	//output.position.x += offset.x;
	//output.position.y += offset.y;
	//output.position.xy *= offset.z;

	// modulate the color
	//output.color *= colorMod;

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}