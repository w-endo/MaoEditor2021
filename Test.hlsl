Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

cbuffer gloabal
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4	 diffuseColor;
	bool		isTexture;
};

struct VS_OUT
{
	float4 pos	 : SV_POSITION;
	float4 color : COLOR;
	float2 uv	 : TEXCOORD;
};


VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	VS_OUT outData;

	float4 light = float4(1, 0, 0, 0);

	normal = mul(normal, matNormal);

	outData.pos = mul(pos, matWVP);
	outData.color = dot(normal, light);

	outData.uv = uv;

	//outData.color *= diffuseColor;
	//outData.color *= g_texture.Sample(g_sampler, uv);

	return outData;
}

float4 PS(VS_OUT inData) : SV_Target
{
	if (isTexture)
	{
		return inData.color *= g_texture.Sample(g_sampler, inData.uv);
	}
	return inData.color *= diffuseColor;
}