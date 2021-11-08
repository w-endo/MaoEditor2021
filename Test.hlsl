cbuffer gloabal
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4	 diffuseColor;
};

struct VS_OUT
{
	float4 pos	 : SV_POSITION;
	float4 color : COLOR;
};


VS_OUT VS(float4 pos : POSITION, float4 normal : NORMAL)
{
	VS_OUT outData;

	float4 light = float4(1, 0, 0, 0);

	normal = mul(normal, matNormal);

	outData.pos = mul(pos, matWVP);
	outData.color = dot(normal, light);

	outData.color *= diffuseColor;

	return outData;
}

float4 PS(VS_OUT inData) : SV_Target
{
	return inData.color;
}