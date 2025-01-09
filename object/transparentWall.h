#pragma once
#include "object/gameObject.h"

// ëOï˚êÈåæ
class BoxCollisionComponent;
class TransparentWall :public GameObject
{
private:
	BoxCollisionComponent* m_BoxCollCache = nullptr;
 public:
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale);
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale,const XMFLOAT3& rot);

	 virtual void Init()override;
};