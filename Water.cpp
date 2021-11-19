#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Water.h"

//�R���X�g���N�^
Water::Water(GameObject* parent)
    :GameObject(parent, "Water"), hModel_(-1)
{
}

//�f�X�g���N�^
Water::~Water()
{
}

//������
void Water::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets\\Water.fbx", SHADER_WATER);
    assert(hModel_ >= 0);

    transform_.scale_.x = 10.0f;
    transform_.scale_.y = 10.0f;
    transform_.scale_.z = 10.0f;
}

//�X�V
void Water::Update()
{
}

//�`��
void Water::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Water::Release()
{
}