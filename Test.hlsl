Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

cbuffer gloabal
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4	 diffuseColor;
	float4	 camPos;
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
	float4 light = float4(1, 0, 0, 0);

	float4 normal = normalize(inData.normal);

	float4 diffuse = saturate( dot(normal, light) );
	if (isTexture)
	{
		diffuse *= g_texture.Sample(g_sampler, inData.uv);
	}
	else
	{
		diffuse *= diffuseColor;
	}


	float4 R = normalize(reflect(-light, normal));
	float ks = 2;
	float shininess =8;
	float4 is = float4(1, 1, 1, 1);
	float4 specular = ks * pow( saturate(  dot(R, normalize(inData.eye))  ), shininess) * is;

	return diffuse + specular;

}