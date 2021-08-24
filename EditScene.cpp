#include "EditScene.h"
#include "Stage.h"
#include "Controller.h"

//コンストラクタ
EditScene::EditScene(GameObject* parent)
	: GameObject(parent, "EditScene")
{
}

//初期化
void EditScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Controller>(this);
}

//更新
void EditScene::Update()
{
}

//描画
void EditScene::Draw()
{

}

//開放
void EditScene::Release()
{
}