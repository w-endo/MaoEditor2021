#include "SampleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Torus.h"

//コンストラクタ
SampleScene::SampleScene(GameObject* parent)
	: GameObject(parent, "SampleScene")
{
}

//初期化
void SampleScene::Initialize()
{
	Instantiate<Torus>(this);
}

//更新
void SampleScene::Update()
{

}

//描画
void SampleScene::Draw()
{
}

//開放
void SampleScene::Release()
{
}