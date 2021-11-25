Texture2D		g_texture : register(t0);	//テクスチャー
Texture2D		g_toonTexture : register(t1);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

cbuffer gloabal
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4	 diffuseColor;
	float4	 speculer;
	float4	 camPos;
	float	 shininess;
	bool		isTexture;
};

struct VS_OUT
{
	float4 pos	 : SV_POSITION;
	float2 uv	 : TEXCOORD;
	float4 eye	 : TEXCOORD1;
	float4 normal: NORMAL;
};


VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	VS_OUT outData;

	normal.w = 0;
	outData.normal = mul(normal, matNormal);

	outData.pos = mul(pos, matWVP);

	outData.eye = normalize(camPos - pos);
	outData.uv = uv;


	return outData;
}

float4 PS(VS_OUT inData) : SV_Target
{
	float4 light = normalize(float4(1, 1, -1, 0));

	float4 normal = normalize(inData.normal);

	float2 uv = float2(saturate(dot(normal, light)), 0);
	float4 diffuse = g_toonTexture.Sample(g_sampler, uv);


	float4 ambient;

	if (isTexture)
	{
		diffuse *= g_texture.Sample(g_sampler, inData.uv);
		ambient = g_texture.Sample(g_sampler, inData.uv) * float4(0.2, 0.2, 0.2, 1);
	}
	else
	{
		diffuse *= diffuseColor;
		ambient = diffuseColor * float4(0.2, 0.2, 0.2, 1);
	}


	float4 R = normalize(reflect(-light, normal));
	float ks = 2;
	float4 specular = ks * pow(saturate(dot(R, normalize(inData.eye))), shininess) * speculer;

	return diffuse + specular + ambient;

}



float4 VS_Outline(float4 pos : POSITION,  float4 normal : NORMAL) : SV_POSITION
{
	pos.x += normal.x * 0.4;
	pos.y += normal.y * 0.4;
	pos.z += normal.z * 0.4;

	return mul(pos, matWVP);
}

float4 PS_Outline(float4 pos : SV_POSITION) : SV_Target
{
	return float4(0, 0, 0, 1);
}