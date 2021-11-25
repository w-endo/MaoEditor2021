#pragma once
#include <d3d11.h>
#include <assert.h>
#include "Texture.h"
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum SHADER_TYPE { SHADER_3D, SHADER_2D, SHADER_TEST, SHADER_WATER, SHADER_TOON, SHADER_OUTLINE, SHADER_MAX };


namespace Direct3D
{
	extern ID3D11Device* pDevice;						//�f�o�C�X
	extern ID3D11DeviceContext* pContext;				//�f�o�C�X�R���e�L�X�g
	extern int screenWidth;
	extern int screenHeight;
	extern Texture* pToonTexture;


	//������
	void Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	void InitShader2D();
	void InitShader3D();
	void InitShaderTest();
	void InitShaderWater();
	void InitShaderToon();
	void InitShaderOutline();

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	void SetShader(SHADER_TYPE type);
};