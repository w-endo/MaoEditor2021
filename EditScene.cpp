#include "EditScene.h"
#include "Stage.h"

//�R���X�g���N�^
EditScene::EditScene(GameObject* parent)
	: GameObject(parent, "EditScene")
{
}

//������
void EditScene::Initialize()
{
}

//�X�V
void EditScene::Update()
{
}

//�`��
void EditScene::Draw()
{
	Instantiate<Stage>(this);
}

//�J��
void EditScene::Release()
{
}