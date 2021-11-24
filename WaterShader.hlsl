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
	float	scroll;
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


	tangent = normalize(mul(tangent, matNormal));
	binormal = normalize(mul(binormal, matNormal));
	normal = normalize(mul(normal, matNormal));

	float4 light = normalize(float4(0.0, 2, 1, 0));
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
	float2 normalUV1 = float2(inData.uv.x + scroll, inData.uv.y + scroll / 2);
	float2 normalUV2 = float2(inData.uv.x - scroll, inData.uv.y - scroll / 2);

	inData.light = normalize(inData.light);
	float4 normal1 = g_textureNormal.Sample(g_sampler, normalUV1 * 1.5) * 2 - 1;
	float4 normal2 = g_textureNormal.Sample(g_sampler, normalUV2 * -1.3) * 2 - 1;
	float4 normal = normal1 + normal2;
	normal = normalize(normal);

	float4 diffuse = saturate(dot(normal, inData.light));
	float4 ambient;

	if (isTexture)
	{
		float2 uvDiff = float2(inData.uv.x + scroll*0.8, inData.uv.y + scroll *0.3);

		diffuse *= g_texture.Sample(g_sampler, uvDiff *2);
		ambient = g_texture.Sample(g_sampler, uvDiff *2) * float4(0.2, 0.2, 0.2, 1);
	}
	else
	{
		diffuse *= diffuseColor;
		ambient = diffuseColor * float4(0.2, 0.2, 0.2, 1);
	}


	float4 R = normalize(reflect(-inData.light, normal));
	float ks = 40;
	float4 specular = ks * pow(saturate(dot(R, normalize(inData.eye))), 40) * speculer;

	float4 color = diffuse + specular + ambient;
	color.a = (color.r + color.g + color.b) / 3;
	color.a *= color.a;
	color.a *= color.a;
	return color;
}