#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Texture.h"
#include "Direct3D.h"

#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

//レイキャスト用構造体
struct RayCastData
{
	XMFLOAT3	start;	//レイ発射位置
	XMFLOAT3	dir;	//レイの向きベクトル
	float		dist;	//衝突点までの距離
	BOOL        hit;	//レイが当たったか
};

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture*	pTexture;
		Texture*	pTextureNormal;
		XMFLOAT4	diffuse;
		XMFLOAT4	specular;
		float		shininess;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;
		XMFLOAT4	specular;
		XMFLOAT4	camPos;
		float		shininess;
		int			isTexture;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
		XMVECTOR tangent;
	};

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数
	int* indexCountEachMaterial_;	//マテリアルごとのポリゴン数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;

	VERTEX* pVertices_;
	int** ppIndex_;

	SHADER_TYPE shaderType_;

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void IntConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
public:

	Fbx();
	HRESULT Load(std::string fileName, SHADER_TYPE shaderType);
	void    Draw(Transform& transform);
	void    Release();

	void RayCast(RayCastData* rayData);
};