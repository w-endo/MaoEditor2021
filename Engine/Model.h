#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx;
		Transform transform;
		std::string fileName;
	};

	



	//モデルをロード
	//引数：fileName　ファイル名
	//戻値：そのモデルデータに割り当てられた番号
	int Load(std::string fileName);

	//ワールド行列を設定
	//引数：handle		設定したいモデルの番号
	//引数：transform	移動・回転・拡大情報
	//戻値：なし
	void SetTransform(int handle, Transform transform);


	void Draw(int handle);


	void AllRelease();

	void RayCast(int handle, RayCastData* rayData)
};

