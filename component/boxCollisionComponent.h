#pragma once
#include "component/collisionComponent.h"

class BoxCollisionComponent final :public CollisionComponent
{
private:
	XMVECTOR m_Mtv = {};

	void GetMyObb(OBB& myObb);

public:
	using CollisionComponent::CollisionComponent;
	virtual void Update()override;
	virtual bool CheckHitObject()override;

	const XMVECTOR& GetMtv()
	{
		return m_Mtv;
	}
};