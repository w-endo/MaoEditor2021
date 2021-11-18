Texture2D		g_texture : register(t0);		//テクスチャー
Texture2D		g_textureNormal : register(t1);	//テクスチャー
SamplerState	g_sampler : register(s0);		//サンプラー

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
	float4 light : TEXCOORD2;
};


VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	VS_OUT outData;

	float3 binormal = cross(normal, tangent);

	tangent.w = 0;
	//binormal.w = 0;
	normal.w = 0;

	
	tangent = mul(tangent, matNormal);
	binormal = mul(binormal, matNormal);
	normal = mul(normal, matNormal);

	float4 light = normalize(float4(1, 1, -1, 0));
	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);


	outData.pos = mul(pos, matWVP);

	float4 eye = normalize(camPos - pos);
	outData.eye.x = dot(eye, tangent);
	outData.eye.y = dot(eye, binormal);
	outData.eye.z = dot(eye, normal);

	outData.uv = uv;

	return outData;
}

float4 PS(VS_OUT inData) : SV_Target
{
	

	//float4 normal = normalize(inData.normal);
	float4 normal = g_textureNormal.Sample(g_sampler, inData.uv) * 2 - 1;
	normal = normalize(normal);

	float4 diffuse = saturate(dot(normal, inData.light));
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


	float4 R = normalize(reflect(-inData.light, normal));
	float ks = 2;
	float4 specular = ks * pow(saturate(dot(R, normalize(inData.eye))), shininess) * speculer;

	return diffuse + specular + ambient;

}