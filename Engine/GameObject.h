#pragma once

#include <string>
#include <list>
#include "Transform.h"
#include "Collider.h"

class GameObject
{
protected:
	std::list<GameObject*> childList_;
	Transform	transform_;
	GameObject* pParent_;
	std::string	objectName_;
	bool isDead;
	Collider* pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }

	void KillMe();
	GameObject* FindChildObject(std::string name);
	GameObject* GetRootJob();
	GameObject* FindObject(std::string name);


	void AddCollider(Collider* pCollider);


	void RecursiveCollison(GameObject* pTarget);
	void Collision(GameObject* pTarget);

	virtual void OnCollision(GameObject* pTarget) {};


	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		childList_.push_back(p);
		return p;
	}
};