#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Torus : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    Torus(GameObject* parent);

    //�f�X�g���N�^
    ~Torus();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};