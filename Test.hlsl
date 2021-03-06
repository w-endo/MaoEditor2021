Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

cbuffer gloabal
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4	 diffuseColor;
	float4	 speculer;
	float4	 camPos;
	float	 shininess;
	bool	isTexture;
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
	float4 light = normalize( float4(1, 1, -1, 0) );

	float4 normal = normalize(inData.normal);

	float4 diffuse = saturate( dot(normal, light) );
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
	float4 specular = ks * pow( saturate(  dot(R, normalize(inData.eye))  ), shininess) * speculer;

	return diffuse + specular + ambient;

}