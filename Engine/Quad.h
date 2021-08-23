#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

using namespace DirectX;

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;
};

//頂点情報
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};

class Quad
{
protected:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ	
	Texture* pTexture_;				//テクスチャ
	int indexCount_;				//インデックス情報の数


	virtual HRESULT CreateVertex();		//頂点バッファ作成
	virtual HRESULT CreateIndex();		//インデックスバッファ作成
	virtual HRESULT CreateConstant();	//コンスタントバッファ作成
	virtual HRESULT CreateTexture();	//テクスチャ作成


public:
	Quad();
	~Quad();
	virtual HRESULT Initialize();
	virtual void Draw(Transform transform);
	virtual void Release();
};