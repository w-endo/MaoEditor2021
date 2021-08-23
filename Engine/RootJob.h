#pragma once
#include "GameObject.h"

class RootJob : public GameObject
{
public:
	RootJob();
	~RootJob();
	void Initialize();
	void Update();
	void Draw();
	void Release();
};

