#include "Sprite.h"
#include "Camera.h"

Sprite::Sprite() :
	pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr),
	indexCount_(0)
{
}

Sprite::~Sprite()
{
}

HRESULT Sprite::Initialize(std::string fileName)
{
	// 頂点情報
	if (FAILED(CreateVertex()))
	{
		return E_FAIL;
	}

	//インデックス情報
	if (FAILED(CreateIndex()))
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstant()))
	{
		return E_FAIL;
	}

	//テクスチャ作成
	if (FAILED(CreateTexture(fileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Sprite::Initialize(ID3D11Texture2D* pTexture)
{
	// 頂点情報
	if (FAILED(CreateVertex()))
	{
		return E_FAIL;
	}

	//インデックス情報
	if (FAILED(CreateIndex()))
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstant()))
	{
		return E_FAIL;
	}

	pTexture_ = new Texture;
	pTexture_->Load(pTexture);

	return S_OK;
}

HRESULT Sprite::CreateTexture(std::string fileName)
{
	pTexture_ = new Texture;
	if (FAILED(pTexture_->Load(fileName.c_str())))
	{
		MessageBox(nullptr, "画像ファイルの読み込みに失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Sprite::CreateConstant()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	if (FAILED(Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Sprite::CreateIndex()
{
	int index[] = { 0,2,3, 0,1,2 };
	indexCount_ = sizeof(index) / sizeof(int);


	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Sprite::CreateVertex()
{
	VERTEX vertices[] =
	{
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),	// 四角形の頂点（左上）
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),	// 四角形の頂点（右上）
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),	// 四角形の頂点（右下）
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),	// 四角形の頂点（左下）		
	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;


	if (FAILED(Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_)))
	{
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void Sprite::Draw(Transform transform)
{
	Direct3D::SetShader(SHADER_2D);

	XMMATRIX scaleMatrix = XMMatrixScaling((float)pTexture_->GetWidth() / Direct3D::screenWidth, (float)pTexture_->GetHeight() / Direct3D::screenHeight, 1.0f);;



	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	cb.matW = XMMatrixTranspose(scaleMatrix * transform.GetWorldMatrix());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る


	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);





	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(indexCount_, 0, 0);
}

void Sprite::Release()
{
	pTexture_->Release();
	SAFE_DELETE(pTexture_);

	pConstantBuffer_->Release();
	pIndexBuffer_->Release();
	pVertexBuffer_->Release();
}