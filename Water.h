#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"

//���������Ǘ�����N���X
class Water : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    Water(GameObject* parent);

    //�f�X�g���N�^
    ~Water();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};