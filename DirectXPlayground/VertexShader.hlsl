cbuffer ConstantBuffer{
	float redLevel;
	float blueLevel;
}

struct VOut {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VOut main(float4 pos : POSITION, float4 color: COLOR)
{
	VOut output;
	output.position = pos;
	output.color = color;

	// modify red and blue channel in each vertex according to our constant buffer
	output.color.r *= redLevel;
	output.color.b *= blueLevel;

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}