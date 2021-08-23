#include "EditScene.h"
#include "Stage.h"

//コンストラクタ
EditScene::EditScene(GameObject* parent)
	: GameObject(parent, "EditScene")
{
}

//初期化
void EditScene::Initialize()
{
}

//更新
void EditScene::Update()
{
}

//描画
void EditScene::Draw()
{
	Instantiate<Stage>(this);
}

//開放
void EditScene::Release()
{
}