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

//���C�L���X�g�p�\����
struct RayCastData
{
	XMFLOAT3	start;	//���C���ˈʒu
	XMFLOAT3	dir;	//���C�̌����x�N�g��
	float		dist;	//�Փ˓_�܂ł̋���
	BOOL        hit;	//���C������������
};

class Fbx
{
	//�}�e���A��
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

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�
	int* indexCountEachMaterial_;	//�}�e���A�����Ƃ̃|���S����

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