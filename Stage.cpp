#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Fbx.h"
#include "resource.h"

//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")//, hModel_(-1)
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            table_[x][z].type = 0;
            table_[x][z].height = 1;
        }
    }

    table_[1][1].type = 2;
    table_[3][5].type = 1;
    table_[10][10].type = 4;

    table_[3][0].height = 3;
    table_[5][2].height = 4;
    table_[10][10].height = 5;
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
    //�}�E�X���N���b�N������
    if (Input::IsMouseButtonDown(0))
    {
        //�r���[�|�[�g�s��
        float w = Direct3D::screenWidth / 2.0f;
        float h = Direct3D::screenHeight / 2.0f;
        XMMATRIX vp = {
            w, 0, 0, 0,
            0, -h, 0, 0,
            0, 0, 1, 0,
            w, h, 0, 1
        };





		//�e�t�s��
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		XMMATRIX invPrj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());


		//�N���b�N�ʒu�i��O�j
        XMVECTOR mousePosFront = { Input::GetMousePosition().x, Input::GetMousePosition().y, 0, 0 };


        //�N���b�N�ʒu�i���j
        XMVECTOR mousePosBack = { Input::GetMousePosition().x, Input::GetMousePosition().y, 1, 0 };


        //�ϊ�
        mousePosFront = XMVector3TransformCoord(mousePosFront, invVP * invPrj * invView);
        mousePosBack = XMVector3TransformCoord(mousePosBack, invVP * invPrj * invView);

        XMFLOAT3 fMPFront;
        XMStoreFloat3(&fMPFront, mousePosFront);


        XMFLOAT3 fDir;
        XMStoreFloat3(&fDir, XMVector3Normalize(mousePosBack - mousePosFront));

        //���C�L���X�g
        for (int x = 0; x < 15; x++)
        {
            for (int z = 0; z < 15; z++)
            {
                for (int y = 0; y < table_[x][z].height; y++)
                {
                    Transform trans;
                    trans.position_.x = x;
                    trans.position_.y = y;
                    trans.position_.z = z;
                    int type = table_[x][z].type;
                    Model::SetTransform(hModel_[type], trans);


                    RayCastData data;
                    data.start = fMPFront;
                    data.dir = fDir;
                    Model::RayCast(hModel_[type], &data);

                    //�����������e�X�g
                    if (data.hit)
                    {
                        switch (status_)
                        {
                        case 0:
                            table_[x][z].height++;
                            break;

                        case 1:
                            table_[x][z].height--;
                            if (table_[x][z].height <= 0)
                            {
                                table_[x][z].height = 1;
                            }
                            break;

                        case 2:
                            table_[x][z].type = select_;
                            break;
                        }

                    }
                }
            }
        }

    }
}

//�`��
void Stage::Draw()
{
    Transform trans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height; y++)
            {
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;

                int type = table_[x][z].type;

                Model::SetTransform(hModel_[type], trans);
                Model::Draw(hModel_[type]);
            }
        }
    }
}

//�J��
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        HWND hCtrl;
        hCtrl = GetDlgItem(hDlg, IDC_RADIO_UP);
        SendMessage(hCtrl, BM_SETCHECK, BST_CHECKED, 0);

        hCtrl = GetDlgItem(hDlg, IDC_COMBO1);
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"����");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(hCtrl, CB_SETCURSEL, 0, 0);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDC_RADIO_UP:
            status_ = 0;
            return TRUE;

        case IDC_RADIO_DOWN:
            status_ = 1;
            return TRUE;

        case IDC_RADIO_CHANGE:
            status_ = 2;
            return TRUE;

        case IDC_COMBO1:
            HWND hCtrl;
            hCtrl = GetDlgItem(hDlg, IDC_COMBO1);
            select_ = (int)SendMessage(hCtrl, CB_GETCURSEL, 0, 0);
            
            return TRUE;
        }
    }
    return FALSE;
}

void Stage::Save()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	            //���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	            //�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);             	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")  //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");             //����
    ofn.lpstrFile = fileName;               	            //�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	                //�p�X�̍ő啶����
    ofn.Flags = OFN_OVERWRITEPROMPT;   		                //�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	            //�f�t�H���g�g���q

    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;



    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        CREATE_ALWAYS,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j

    char s[] = "����ɂ���";

    DWORD dwBytes = 0;  //�������݈ʒu
    WriteFile(
        hFile,              //�t�@�C���n���h��
        s,                  //�ۑ�����f�[�^�i������j
        (DWORD)strlen(s),   //�������ޕ�����
        &dwBytes,           //�������񂾃T�C�Y������ϐ�
        NULL);              //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);
}

void Stage::Load()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

     //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	            //���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	            //�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);             	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")  //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");             //����
    ofn.lpstrFile = fileName;               	            //�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	                //�p�X�̍ő啶����
    ofn.Flags = OFN_FILEMUSTEXIST;   		                //�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	            //�f�t�H���g�g���q

    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_READ,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j


      //�t�@�C���̃T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);

    //�t�@�C���̃T�C�Y�����������m��
    char* data;
    data = new char[fileSize];

    DWORD dwBytes = 0; //�ǂݍ��݈ʒu

    ReadFile(
        hFile,     //�t�@�C���n���h��
        data,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &dwBytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);
}
