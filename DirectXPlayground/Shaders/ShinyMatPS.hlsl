float4 main(float4 position: SV_POSITION, float4 color : COLOR, float4 normal : NORMAL, float3 viewDirection: TEXCOORD1) : SV_TARGET
{
	return color;
}