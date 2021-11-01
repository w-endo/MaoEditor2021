#include "SceneManager.h"
#include "../SampleScene.h"
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
    nowScene_ = SCENE_ID_SAMPLE;
    nextScene_ = SCENE_ID_SAMPLE;
    Instantiate<SampleScene>(this);
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
        case SCENE_ID_SAMPLE: Instantiate<SampleScene>(this); break;

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
