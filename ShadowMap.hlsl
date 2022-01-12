Texture2D		g_texture : register(t0);		//テクスチャー
Texture2D		g_textureNormal : register(t1);	//テクスチャー
SamplerState	g_sampler : register(s0);		//サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4x4 matWorld;
	float4x4	g_mWLP;   //ワールド・”ライトビュー”・プロジェクションの合成 
	float4x4	g_mWLPT;   //ワールド・”ライトビュー”・プロジェクション・UV 行列の合成
	float4	 diffuseColor;
	float4	 speculer;
	float4	 camPos;
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
	float z		: TEXCOORD1;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);

	outData.z = length(camPos - mul(pos, matWorld)) / 30.0;


	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	return float4(inData.z, inData.z, inData.z, 1);

}
