cbuffer ConstantBuffer {
	float4 ambientCol;
	float4 diffuseCol;
	float4 specularCol;
	float4 lightDir;
	float specularPow;
	float3 padding1;
};

float4 main(float4 pos: SV_POSITION, float4 color : COLOR, float4 worldNormal : NORMAL, float3 viewDir: TEXCOORD1) : SV_TARGET
{
	float lightIntensity = saturate(dot(worldNormal, -lightDir)); // calculate amount of light on this pixel
	float4 finalCol = ambientCol;
	float4 specularCol = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (lightIntensity > 0.0f) {
		// determine the final diffuse color based on the diffuse color and light intensity
		finalCol += (diffuseCol * lightIntensity);

		// saturate clamps the specified value between 0 and 1
		finalCol = saturate(finalCol);

		// calculate reflection vector based on light intensity, normal vector, and light direction
		// ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
		float3 reflection = normalize(2 * lightIntensity * worldNormal - lightDir);

		// amount of specular light visible is based on reflection vector and viewing dir
		// the smaller the nagle between viewing dir and reflection vector, the greate the specular color
		// finally, the result is taken to the power of specularPow. The LOWER the specularPow is, the GREATER the final effect
		// SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower
		specularCol = pow(saturate(dot(reflection, viewDir)), specularPow);

	}

	finalCol = ambientCol * color;

	// we don't add the specular effect until the end
	// it is a highlight and needs to be added to the final value or it will not show up correctly
	finalCol = saturate(finalCol + specularCol);

	return finalCol;
}