#include "Math.h"


//s—ñŽ®
float Math::Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
{
	return	(a.x * b.y * c.z) +
		(a.y * b.z * c.x) +
		(a.z * b.x * c.y) -
		(a.x * b.z * c.y) -
		(a.y * b.x * c.z) -
		(a.z * b.y * c.z);
}

bool Math::Intersect(XMFLOAT3 origin, XMFLOAT3 ray, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
{
	XMVECTOR vOrigin = XMLoadFloat3(&origin);
	XMVECTOR vRay = XMLoadFloat3(&ray);
	XMVECTOR vV0 = XMLoadFloat3(&v0);
	XMVECTOR vV1 = XMLoadFloat3(&v1);
	XMVECTOR vV2 = XMLoadFloat3(&v2);

	XMVECTOR vEdge1 = vV1 - vV0;
	XMVECTOR vEdge2 = vV2 - vV0;

	XMFLOAT3 edge1;
	XMFLOAT3 edge2;
	XMStoreFloat3(&edge1, vEdge1);
	XMStoreFloat3(&edge2, vEdge2);

	XMFLOAT3 d;
	XMStoreFloat3(&d, vOrigin - vV0);

	float u = Det(d, edge2, -ray) / Det(edge1, edge2, -ray);
	float v = Det(edge1, d, -ray) / Det(edge1, edge2, -ray);
	float t = Det(edge1, edge2, d) / Det(edge1, edge2, -ray);

	if (u >= 0 && v >= 0 && t >= 0 && (u + v) <= 1.0f)
	{
		return true;
	}
	return false;
}
