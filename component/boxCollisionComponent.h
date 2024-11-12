#pragma once
#include "component/collisionComponent.h"

class BoxCollisionComponent final :public CollisionComponent
{
public:
	using CollisionComponent::CollisionComponent;
	virtual void Update()override;
	virtual bool CheckHitObject()override;
};