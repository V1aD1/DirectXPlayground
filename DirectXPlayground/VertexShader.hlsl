float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4 newPos = {pos.x + 0.5f, pos.y, pos.z, pos.w};
	return newPos;
}