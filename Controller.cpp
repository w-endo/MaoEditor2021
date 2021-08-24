#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

//コンストラクタ
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller")
{
	transform_.position_.x = 7;
	transform_.position_.z = 7;
	transform_.rotate_.x = 45.0f;
}

//デストラクタ
Controller::~Controller()
{
}

//初期化
void Controller::Initialize()
{
}

//更新
void Controller::Update()
{
	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.rotate_.y += 1.0f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.rotate_.y -= 1.0f;
	}

	if (Input::IsKey(DIK_UP))
	{
		transform_.rotate_.x += 1.0f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.rotate_.x -= 1.0f;
	}

	XMMATRIX matY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMMATRIX matX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	XMVECTOR right{ 0.1f, 0, 0, 0 };
	XMVECTOR front{ 0, 0, 0.1f, 0 };
	right = XMVector3TransformCoord(right, matY);
	front = XMVector3TransformCoord(front, matY);


	if (Input::IsKey(DIK_W))
	{
		XMVECTOR v = XMLoadFloat3(&transform_.position_);
		v += front;
		XMStoreFloat3(&transform_.position_, v);
	}
	if (Input::IsKey(DIK_S))
	{
		XMVECTOR v = XMLoadFloat3(&transform_.position_);
		v -= front;
		XMStoreFloat3(&transform_.position_, v);
	}
	if (Input::IsKey(DIK_A))
	{
		XMVECTOR v = XMLoadFloat3(&transform_.position_);
		v -= right;
		XMStoreFloat3(&transform_.position_, v);
	}
	if (Input::IsKey(DIK_D))
	{
		XMVECTOR v = XMLoadFloat3(&transform_.position_);
		v += right;
		XMStoreFloat3(&transform_.position_, v);
	}


	XMVECTOR camArm = XMVectorSet(0, 0, -10, 0);
	camArm = XMVector3TransformCoord(camArm, matX * matY);


	XMVECTOR v = XMLoadFloat3(&transform_.position_);
	v += camArm;

	Camera::SetPosition(v);
	Camera::SetTarget(transform_.position_);
}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}