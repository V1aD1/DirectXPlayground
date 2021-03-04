struct VOut {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

VOut main(float4 pos : POSITION, float4 color: COLOR)
{
	VOut output;
	output.position = pos;
	output.color = color;

	// creating hot colors
	output.color.r = 1.0f;

	// shrinking the triangle
	output.position.xy *= 0.5f;

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}