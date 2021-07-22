cbuffer ConstantBuffer {
	float4 ambientColor;
	float4 diffuseColor;
	float4 lightDirection;
	float specularPower;
	float3 padding1;
	float4 specularColor;
};

float4 main(float4 pos: SV_POSITION, float4 color : COLOR, float4 worldNormal : NORMAL, float3 viewDir: TEXCOORD1) : SV_TARGET
{
	//ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
	//SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower
	return color;
}