#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Fbx.h"
#include "resource.h"

//コンストラクタ
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

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
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

//更新
void Stage::Update()
{
    //マウスをクリックしたら
    if (Input::IsMouseButtonDown(0))
    {
        //ビューポート行列
        float w = Direct3D::screenWidth / 2.0f;
        float h = Direct3D::screenHeight / 2.0f;
        XMMATRIX vp = {
            w, 0, 0, 0,
            0, -h, 0, 0,
            0, 0, 1, 0,
            w, h, 0, 1
        };





		//各逆行列
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		XMMATRIX invPrj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());


		//クリック位置（手前）
        XMVECTOR mousePosFront = { Input::GetMousePosition().x, Input::GetMousePosition().y, 0, 0 };


        //クリック位置（奥）
        XMVECTOR mousePosBack = { Input::GetMousePosition().x, Input::GetMousePosition().y, 1, 0 };


        //変換
        mousePosFront = XMVector3TransformCoord(mousePosFront, invVP * invPrj * invView);
        mousePosBack = XMVector3TransformCoord(mousePosBack, invVP * invPrj * invView);

        XMFLOAT3 fMPFront;
        XMStoreFloat3(&fMPFront, mousePosFront);


        XMFLOAT3 fDir;
        XMStoreFloat3(&fDir, XMVector3Normalize(mousePosBack - mousePosFront));

        //レイキャスト
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

                    //当たったかテスト
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

//描画
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

//開放
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
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"デフォルト");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"草原");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"砂");
        SendMessage(hCtrl, CB_ADDSTRING, 0, (LPARAM)"水");
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
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //─┬ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");             //─┘
    ofn.lpstrFile = fileName;               	            //ファイル名
    ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
    ofn.Flags = OFN_OVERWRITEPROMPT;   		                //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;



    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_WRITE,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        CREATE_ALWAYS,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）

    char s[] = "こんにちは";

    DWORD dwBytes = 0;  //書き込み位置
    WriteFile(
        hFile,              //ファイルハンドル
        s,                  //保存するデータ（文字列）
        (DWORD)strlen(s),   //書き込む文字数
        &dwBytes,           //書き込んだサイズを入れる変数
        NULL);              //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);
}

void Stage::Load()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

     //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //─┬ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");             //─┘
    ofn.lpstrFile = fileName;               	            //ファイル名
    ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
    ofn.Flags = OFN_FILEMUSTEXIST;   		                //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_READ,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        OPEN_EXISTING,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）


      //ファイルのサイズを取得
    DWORD fileSize = GetFileSize(hFile, NULL);

    //ファイルのサイズ分メモリを確保
    char* data;
    data = new char[fileSize];

    DWORD dwBytes = 0; //読み込み位置

    ReadFile(
        hFile,     //ファイルハンドル
        data,      //データを入れる変数
        fileSize,  //読み込むサイズ
        &dwBytes,  //読み込んだサイズ
        NULL);     //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);
}
