#include "SampleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Torus.h"
#include "Water.h"

//�R���X�g���N�^
SampleScene::SampleScene(GameObject* parent)
	: GameObject(parent, "SampleScene")
{
}

//������
void SampleScene::Initialize()
{
	Instantiate<Torus>(this);
	Instantiate<Water>(this);
}

//�X�V
void SampleScene::Update()
{

}

//�`��
void SampleScene::Draw()
{
}

//�J��
void SampleScene::Release()
{
}