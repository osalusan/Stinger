#include "transparentWall.h"
#include "component/boxCollisionComponent.h"

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale)
{
	m_Position = pos;
	m_Scale = scale;
	AddComponent<BoxCollisionComponent>(this);
}
