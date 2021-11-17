#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Torus.h"

//コンストラクタ
Torus::Torus(GameObject* parent)
    :GameObject(parent, "Torus"), hModel_(-1)
{
}

//デストラクタ
Torus::~Torus()
{
}

//初期化
void Torus::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets\\Ball.fbx");
    assert(hModel_ >= 0);

    transform_.scale_.x = 3.0f;
    transform_.scale_.y = 3.0f;
    transform_.scale_.z = 3.0f;
}

//更新
void Torus::Update()
{
    transform_.rotate_.y += 1;
}

//描画
void Torus::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Torus::Release()
{
}