#pragma once
#include "Transform.h"

namespace Math
{
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);

	//�O�p�`�Ɛ����̌�������
	bool Intersect(XMFLOAT3 origin, XMFLOAT3 ray, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
}