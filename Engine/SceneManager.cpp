#include "SceneManager.h"
#include "../EditScene.h"
#include "Model.h"


//�R���X�g���N�^
SceneManager::SceneManager(GameObject* parent)
    :GameObject(parent, "SceneManager")
{
}

//�f�X�g���N�^
SceneManager::~SceneManager()
{
}

//������
void SceneManager::Initialize()
{
    nowScene_ = SCENE_ID_EDIT;
    nextScene_ = SCENE_ID_EDIT;
    Instantiate<EditScene>(this);
}

//�X�V
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

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID nestScene)
{
    nextScene_ = nestScene;
}
