#include "Math.h"


//çsóÒéÆ
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






	return false;
}
