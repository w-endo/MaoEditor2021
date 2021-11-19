#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Water.h"

//コンストラクタ
Water::Water(GameObject* parent)
    :GameObject(parent, "Water"), hModel_(-1)
{
}

//デストラクタ
Water::~Water()
{
}

//初期化
void Water::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets\\Water.fbx", SHADER_WATER);
    assert(hModel_ >= 0);

    transform_.scale_.x = 10.0f;
    transform_.scale_.y = 10.0f;
    transform_.scale_.z = 10.0f;
}

//更新
void Water::Update()
{
}

//描画
void Water::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Water::Release()
{
}