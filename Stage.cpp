#include "Stage.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")//, hModel_(-1)
{
    ZeroMemory(table_, sizeof(table_));

    table_[1][1] = 2;
    table_[3][5] = 1;
    table_[10][10] = 4;
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
}

//描画
void Stage::Draw()
{
    Transform trans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            trans.position_.x = x;
            trans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], trans);
            Model::Draw(hModel_[type]);
        }
    }
}

//開放
void Stage::Release()
{
}