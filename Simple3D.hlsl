//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー
Texture2D		g_textureShadow : register(t1);	//テクスチャー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matNormal;		//法線を変形させる行列
	float4x4	matWorld;
	float4x4	g_mWLP;   //ワールド・”ライトビュー”・プロジェクションの合成 
	float4x4	g_mWLPT;   //ワールド・”ライトビュー”・プロジェクション・UV 行列の合成
	float4	 diffuseColor;
	float4	 speculer;
	float4	 camPos;
	float4	 lightPos;
	float	 shininess;
	bool		isTexture;
	float	scroll;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos  : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;		//UV座標
	float4 color	: COLOR;	//色（明るさ）
	float4 LightTexCoord :  TEXCOORD5;
	float4 LighViewPos :  TEXCOORD6;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;

	//法線を回転
	normal = mul(normal, matNormal);
	normal = normalize(normal);

	float4 light = normalize(lightPos);// float4(-1, 1, -1, 0);
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1);

	//ライトビューを参照するとき、手がかりとなるテクスチャー座標 
	outData.LightTexCoord = mul(pos, g_mWLPT);    //この点が、ライトビューであったときの位置がわかる 

	//ライトビューにおける位置(変換後) 
	outData.LighViewPos = mul(pos, matWorld);

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;
	float4 ambient;
	//if (isTexture)
	//{
	//	diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;
	//	ambient = g_texture.Sample(g_sampler, inData.uv) * float4(0.2, 0.2, 0.2, 1);
	//	diffuse.a = g_texture.Sample(g_sampler, inData.uv).a;
	//}
	//else
	{
		diffuse = diffuseColor * inData.color;
		ambient = diffuseColor * float4(0.2, 0.2, 0.2, 1);
		diffuse.a = 1;
	}

	//影の処理 
	inData.LightTexCoord /= inData.LightTexCoord.w;
	float TexValue = g_textureShadow.Sample(g_sampler, inData.LightTexCoord).r;
	float LightLength = length(inData.LighViewPos - lightPos)/30.0;
	if (TexValue+0.005 < LightLength)  //ライトビューでの長さが短い（ライトビューでは遮蔽物がある） 
	{
		diffuse *=0.0;  //影（明るさを 1/3） 
	}
	//diffuse = LightLength;
	diffuse.a = 1;
	//return diffuse;



	//diffuse = g_textureShadow.Sample(g_sampler, inData.LightTexCoord);
	//diffuse.a = 1;

	return diffuse + ambient;
	
}
