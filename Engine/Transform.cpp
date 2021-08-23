#include "Transform.h"

Transform::Transform()
{
    //メンバ変数６個初期化
    matTranslate_ = XMMatrixIdentity();
    matRotate_ = XMMatrixIdentity();
    matScale_ = XMMatrixIdentity();
    position_ = XMFLOAT3(0, 0, 0);
    rotate_ = XMFLOAT3(0, 0, 0);
    scale_ = XMFLOAT3(1, 1, 1);

}

Transform::~Transform()
{
    //なし
}

void Transform::Calclation()
{
    //移動行列作成
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

    //回転行列作成
    XMMATRIX rotateX, rotateY, rotateZ;
    rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x));
    rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y));
    rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
    matRotate_ = rotateZ * rotateX * rotateY;

    //拡大行列作成
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    Calclation();
    return  matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix()
{
    return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
