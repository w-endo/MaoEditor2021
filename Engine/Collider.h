#pragma once
class Collider
{
	float radius_;
public:
	Collider(float radius);
	float GetRadius() { return radius_; }
};

