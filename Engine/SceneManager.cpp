#include "SceneManager.h"
#include "../EditScene.h"
#include "Model.h"


//コンストラクタ
SceneManager::SceneManager(GameObject* parent)
    :GameObject(parent, "SceneManager")
{
}

//デストラクタ
SceneManager::~SceneManager()
{
}

//初期化
void SceneManager::Initialize()
{
    nowScene_ = SCENE_ID_EDIT;
    nextScene_ = SCENE_ID_EDIT;
    Instantiate<EditScene>(this);
}

//更新
void SceneManager::Update()
{
    if (nowScene_ != nextScene_)
    {
        ((GameObject*)*childList_.begin())->KillMe();

        Model::AllRelease();

        switch (nextScene_)
        {
        case SCENE_ID_EDIT: Instantiate<EditScene>(this); break;

        }

        nowScene_ = nextScene_;

    }
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID nestScene)
{
    nextScene_ = nestScene;
}
