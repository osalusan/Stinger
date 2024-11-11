#pragma once
#include "component/collisionComponent.h"

class BoxCollisionComponent final :public CollisionComponent
{
private:

public:
	using CollisionComponent::CollisionComponent;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
};