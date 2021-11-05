cbuffer gloabal
{
	float4x4 matWVP;
};


float4 VS(float4 pos : POSITION) : SV_POSITION
{
	float4 p = mul(pos, matWVP);
	return p;
}

float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	return float4(1, 1, 1, 1);
}