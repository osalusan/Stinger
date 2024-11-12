#pragma once
#include "component/collisionComponent.h"

class BoxCollisionComponent final :public CollisionComponent
{
private:
	void GetMyObb(OBB& myObb);

public:
	using CollisionComponent::CollisionComponent;
	virtual void Update()override;
	virtual bool CheckHitObject()override;
};