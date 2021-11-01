#pragma once
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
    SCENE_ID_SAMPLE = 0,
};

//◆◆◆を管理するクラス
class SceneManager : public GameObject
{
    SCENE_ID nowScene_;
    SCENE_ID nextScene_;

public:
    //コンストラクタ
    SceneManager(GameObject* parent);

    //デストラクタ
    ~SceneManager();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void ChangeScene(SCENE_ID nestScene);
};