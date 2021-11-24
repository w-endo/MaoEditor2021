#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Torus.h"

//�R���X�g���N�^
Torus::Torus(GameObject* parent)
    :GameObject(parent, "Torus"), hModel_(-1)
{
}

//�f�X�g���N�^
Torus::~Torus()
{
}

//������
void Torus::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets\\Ball.fbx", SHADER_TOON);
    assert(hModel_ >= 0);

    transform_.scale_.x = 2.0f;
    transform_.scale_.y = 2.0f;
    transform_.scale_.z = 2.0f;
}

//�X�V
void Torus::Update()
{
    transform_.rotate_.y += 1;
}

//�`��
void Torus::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Torus::Release()
{
}