#include "transparentWall.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale)
{
	m_Position = pos;
	m_Scale = scale;
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::OBJECT);
}

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot)
	:TransparentWall(pos,scale)
{
	m_Rotation = rot;
}

void TransparentWall::Init()
{
	// モデルが無いので、架空のモデルを使用
	// GameSceneの最大行動範囲に合わせる為、スケールを2倍にして対応
	float customScale = DEFAULT_MODELSCALE * 2.0f;
	m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { customScale,customScale,customScale }, GetRotationMatrix());
}
