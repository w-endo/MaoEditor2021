#include "Fbx.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Math.h"

Fbx::Fbx():
	vertexCount_(0), polygonCount_(0),
	pVertexBuffer_(nullptr),pIndexBuffer_(nullptr),pConstantBuffer_(nullptr)
{
}

HRESULT Fbx::Load(std::string fileName, SHADER_TYPE shaderType)
{
	shaderType_ = shaderType;

	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();


	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//�e���̌����擾
	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�
	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�
	materialCount_ = pNode->GetMaterialCount();

	indexCountEachMaterial_ = new int[materialCount_];

	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//������fileName����f�B���N�g���������擾
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);

	//�J�����g�f�B���N�g���ύX
	SetCurrentDirectory(dir);


	InitVertex(mesh);		//���_�o�b�t�@����
	InitIndex(mesh);		//�C���f�b�N�X�o�b�t�@����
	IntConstantBuffer();	//�R���X�^���g�o�b�t�@����
	InitMaterial(pNode);


	//�J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectory(defaultCurrentDir);

	//�}�l�[�W�����
	pFbxManager->Destroy();

	return S_OK;
}

//���_�o�b�t�@����
void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��
	pVertices_ = new VERTEX[vertexCount_];

	//�S�|���S��
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			pVertices_[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//���_��UV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			pVertices_[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//���_�̖@��
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
			pVertices_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}


	//�^���W�F���g�擾
	for (int i = 0; i < polygonCount_; i++)
	{
		int startIndex = mesh->GetPolygonVertexIndex(i);

		FbxGeometryElementTangent* t = mesh->GetElementTangent(0);
		FbxVector4 tangent = t->GetDirectArray().GetAt(startIndex).mData;

		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertices()[startIndex + j];
			pVertices_[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f);
		}
	}



	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertices_;


	if (FAILED(Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_)))
	{
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}

}

//�C���f�b�N�X�o�b�t�@����
void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	
	ppIndex_ = new int* [materialCount_];

	

	for (int i = 0; i < materialCount_; i++)
	{
		indexCountEachMaterial_[i] = 0;
		ppIndex_[i] = new int[polygonCount_ * 3];

		int count = 0;

		//�S�|���S��
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					ppIndex_[i][count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}

				indexCountEachMaterial_[i]++;
			}
		}

		// �C���f�b�N�X�o�b�t�@�𐶐�����
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = ppIndex_[i];
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i])))
		{
			MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		}
	}
}

void Fbx::IntConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	if (FAILED(Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_)))
	{
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		//�}�e���A���̐F
		FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
		FbxDouble3  diffuse = pMaterial->Diffuse;
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);

		pMaterialList_[i].specular = XMFLOAT4(0, 0, 0, 0);
		pMaterialList_[i].shininess = 1;

		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxDouble3 specular = pMaterial->Specular;
			pMaterialList_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
			pMaterialList_[i].shininess = (float)pMaterial->Shininess;
		}

		//�f�B�t���[�Y�e�N�X�`��
		{
			//�e�N�X�`�����
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

			//�e�N�X�`���̐���
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
			pMaterialList_[i].pTexture = nullptr;

			//�e�N�X�`������
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//�t�@�C����+�g�������ɂ���
				char name[_MAX_FNAME];	//�t�@�C����
				char ext[_MAX_EXT];	//�g���q
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//�t�@�C������e�N�X�`���쐬
				pMaterialList_[i].pTexture = new Texture;
				pMaterialList_[i].pTexture->Load(name);
			}
		}

		//�m�[�}���e�N�X�`��
		{
			//�e�N�X�`�����
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);

			//�e�N�X�`���̐���
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
			pMaterialList_[i].pTextureNormal = nullptr;

			//�e�N�X�`������
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//�t�@�C����+�g�������ɂ���
				char name[_MAX_FNAME];	//�t�@�C����
				char ext[_MAX_EXT];	//�g���q
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//�t�@�C������e�N�X�`���쐬
				pMaterialList_[i].pTextureNormal = new Texture;
				pMaterialList_[i].pTextureNormal->Load(name);
			}
		}

	}
}

void Fbx::Draw(Transform& transform)
{
	//Direct3D::SetShader(shaderType_);



	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);



	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

	for (int i = 0; i < materialCount_; i++)
	{
		// �C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);


		//�R���X�^���g�o�b�t�@�ɓn�����
		CONSTANT_BUFFER cb;
		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
		cb.matWorld = XMMatrixTranspose(transform.GetWorldMatrix());
		cb.isTexture = pMaterialList_[i].pTexture != nullptr;
		cb.diffuseColor = pMaterialList_[i].diffuse;
		cb.specular = pMaterialList_[i].specular;
		cb.shininess = pMaterialList_[i].shininess;
		cb.camPos = XMFLOAT4(Camera::GetPosition().x, Camera::GetPosition().y, Camera::GetPosition().z, 0);
		cb.lightPos = XMFLOAT4(15.0f, 3.0f, -5.0f, 0.0);
		cb.matWLP = XMMatrixTranspose(transform.GetWorldMatrix() * Direct3D::lightViewMatrix * Camera::GetProjectionMatrix());
		cb.matWLPT = XMMatrixTranspose(transform.GetWorldMatrix() * Direct3D::lightViewMatrix * Camera::GetProjectionMatrix() * Direct3D::clipToUVMatrix);

		if (shaderType_ == SHADER_WATER)
		{
			static float scroll = 0.0f;
			//scroll += 0.001f;
			cb.scroll = scroll;
		}


		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

		//�f�B�t���[�Y�e�N�X�`��
		if (pMaterialList_[i].pTexture != nullptr)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
		}

		Direct3D::pContext->PSSetShaderResources(1, 1, &Direct3D::pRenderTargetSRV);



		////�m�[�}���e�N�X�`��
		//if (pMaterialList_[i].pTextureNormal != nullptr)
		//{
		//	ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTextureNormal->GetSRV();
		//	Direct3D::pContext->PSSetShaderResources(1, 1, &pSRV);
		//}

		////�g�D�[���e�N�X�`��
		//if (shaderType_ == SHADER_TOON)
		//{
		//	ID3D11ShaderResourceView* pSRV = Direct3D::pToonTexture->GetSRV();
		//	Direct3D::pContext->PSSetShaderResources(1, 1, &pSRV);
		//}




		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ


		Direct3D::pContext->DrawIndexed(polygonCount_ * 3, 0, 0);
	}

	if (shaderType_ == SHADER_OUTLINE)
	{
		shaderType_ = SHADER_TOON;
		Draw(transform);
		shaderType_ = SHADER_OUTLINE;
	}


}

void Fbx::Release()
{
}

void Fbx::RayCast(RayCastData* rayData)
{
	//�}�e���A����
	for (DWORD i = 0; i < materialCount_; i++)
	{
		//���̃}�e���A���̃|���S����
		for (DWORD j = 0; j < indexCountEachMaterial_[i] / 3; j++)
		{
			//3���_
			XMVECTOR v0 = pVertices_[ppIndex_[i][j * 3 + 0]].position;
			XMVECTOR v1 = pVertices_[ppIndex_[i][j * 3 + 1]].position;
			XMVECTOR v2 = pVertices_[ppIndex_[i][j * 3 + 2]].position;

			XMFLOAT3 fV0;
			XMFLOAT3 fV1;
			XMFLOAT3 fV2;
			XMStoreFloat3(&fV0, v0);
			XMStoreFloat3(&fV1, v1);
			XMStoreFloat3(&fV2, v2);

			//���̎O�p�`�Ƃ̔���
			rayData->hit = Math::Intersect(rayData->start, rayData->dir, fV0, fV1, fV2);

			//�������Ă���I���
			if (rayData->hit)
			{
				return;
			}
		}
	}
}