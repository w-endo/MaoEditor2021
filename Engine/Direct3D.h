#pragma once
#include <d3d11.h>
#include <assert.h>
#include "Texture.h"
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

#include <DirectXMath.h>

using namespace DirectX;


//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum SHADER_TYPE { SHADER_3D, SHADER_2D, SHADER_TEST, SHADER_WATER, SHADER_TOON, SHADER_OUTLINE, SHADER_SHADOWMAP, SHADER_MAX };


namespace Direct3D
{
	extern ID3D11Device* pDevice;						//デバイス
	extern ID3D11DeviceContext* pContext;				//デバイスコンテキスト
	extern int screenWidth;
	extern int screenHeight;
	extern Texture* pToonTexture;
	extern XMMATRIX lightViewMatrix;
	extern XMMATRIX clipToUVMatrix;
	extern ID3D11ShaderResourceView* pRenderTargetSRV;


	//初期化
	void Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	void InitShader2D();
	void InitShader3D();
	void InitShaderTest();
	void InitShaderWater();
	void InitShaderToon();
	void InitShaderOutline();
	void InitShaderShadowMap();

	//描画開始
	void BeginDraw();
	void BeginDraw2();

	void ScreenDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	void SetShader(SHADER_TYPE type);
};