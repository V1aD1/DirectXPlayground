cbuffer ConstantBuffer {
	float4x4 final; // todo get rid of this
	float4x4 rotation; // todo get rid of this
	float4 materialCol;

	float3 cameraPos;
	float padding;

	// testing
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

struct VOut {
	float4 pos: SV_POSITION;
	float4 color: COLOR;
	float4 worldNormal: NORMAL;
	float3 viewDir: TEXCOORD1;
};

VOut main(float4 vPos : POSITION, float4 vNormal : NORMAL)
{
	// order of multiplication matters!!
	float4 worldPos = mul(worldMatrix, vPos); // mul is an intrinsic that also handles matrix multiplication

	VOut output;
	output.pos = mul(viewMatrix, worldPos);
	output.pos = mul(projectionMatrix, output.pos);
	output.color = materialCol; // final color
	output.worldNormal = normalize(mul(worldMatrix, vNormal)); // finding out the normal in world space of the vertex
	output.viewDir = normalize(cameraPos.xyz - worldPos.xyz); //ViewingDirection = CameraPosition - VertexPosition

	// order of return values MUST MATCH order of input variables to pixel shader!
	return output;
}