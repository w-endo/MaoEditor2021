#pragma once
#include <Windows.h>
#include "Engine/GameObject.h"

//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_[5];    //モデル番号

    struct
    {
        int type;
        int height;
    }table_[15][15];

    int status_;    //上げる？　下げる？　変える？
    int select_;    


public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    void Save();

    void Load();
};