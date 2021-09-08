#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Fbx.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")//, hModel_(-1)
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            table_[x][z].type = 0;
            table_[x][z].height = 1;
        }
    }

    table_[1][1].type = 2;
    table_[3][5].type = 1;
    table_[10][10].type = 4;

    table_[3][0].height = 3;
    table_[5][2].height = 4;
    table_[10][10].height = 5;
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_[0] = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Assets/BoxBrick.fbx");
    assert(hModel_[1] >= 0);

    hModel_[2] = Model::Load("Assets/BoxGrass.fbx");
    assert(hModel_[2] >= 0);

    hModel_[3] = Model::Load("Assets/BoxSand.fbx");
    assert(hModel_[3] >= 0);

    hModel_[4] = Model::Load("Assets/BoxWater.fbx");
    assert(hModel_[4] >= 0);
}

//更新
void Stage::Update()
{
    //マウスをクリックしたら
    if (Input::IsMouseButtonDown(0))
    {
        //ビューポート行列
        float w = Direct3D::screenWidth / 2.0f;
        float h = Direct3D::screenHeight / 2.0f;
        XMMATRIX vp = {
            w, 0, 0, 0,
            0, -h, 0, 0,
            0, 0, 1, 0,
            w, h, 0, 1
        };





		//各逆行列
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		XMMATRIX invPrj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());


		//クリック位置（手前）
        XMVECTOR mousePosFront = { Input::GetMousePosition().x, Input::GetMousePosition().y, 0, 0 };


        //クリック位置（奥）
        XMVECTOR mousePosBack = { Input::GetMousePosition().x, Input::GetMousePosition().y, 1, 0 };


        //変換
        mousePosFront = XMVector3TransformCoord(mousePosFront, invVP * invPrj * invView);
        mousePosBack = XMVector3TransformCoord(mousePosBack, invVP * invPrj * invView);

        XMFLOAT3 fMPFront;
        XMStoreFloat3(&fMPFront, mousePosFront);


        XMFLOAT3 fDir;
        XMStoreFloat3(&fDir, XMVector3Normalize(mousePosBack - mousePosFront));

        //レイキャスト
        for (int x = 0; x < 15; x++)
        {
            for (int z = 0; z < 15; z++)
            {
                for (int y = 0; y < table_[x][z].height; y++)
                {
                    Transform trans;
                    trans.position_.x = x;
                    trans.position_.y = y;
                    trans.position_.z = z;
                    int type = table_[x][z].type;
                    Model::SetTransform(hModel_[type], trans);


                    RayCastData data;
                    data.start = fMPFront;
                    data.dir = fDir;
                    Model::RayCast(hModel_[type], &data);

                    //当たったかテスト
                    if (data.hit)
                    {
                        
                        table_[x][z].height++;

                    }
                }
            }
        }

    }
}

//描画
void Stage::Draw()
{
    Transform trans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height; y++)
            {
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;

                int type = table_[x][z].type;

                Model::SetTransform(hModel_[type], trans);
                Model::Draw(hModel_[type]);
            }
        }
    }
}

//開放
void Stage::Release()
{
}