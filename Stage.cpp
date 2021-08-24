#include "Stage.h"
#include "Engine/Model.h"

//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")//, hModel_(-1)
{
    ZeroMemory(table_, sizeof(table_));

    table_[1][1] = 2;
    table_[3][5] = 1;
    table_[10][10] = 4;
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_[0] = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Assets/BoxBrick.fbx");
    assert(hModel_[1] >= 0);

    hModel_[2] = Model::Load("Assets/BoxGrass.fbx");
    assert(hModel_[2] >= 0);

    hModel_[3] = Model::Load("Assets/BoxSand.fbx");
    assert(hModel_[3] >= 0);

    hModel_[4] = Model::Load("Assets/BoxWater.fbx");
    assert(hModel_[4] >= 0);
}

//�X�V
void Stage::Update()
{
}

//�`��
void Stage::Draw()
{
    Transform trans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            trans.position_.x = x;
            trans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], trans);
            Model::Draw(hModel_[type]);
        }
    }
}

//�J��
void Stage::Release()
{
}