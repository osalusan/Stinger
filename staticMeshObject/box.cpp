#include "box.h"
#include "manager/objModelManager.h"
#include "component/boxCollisionComponent.h"

Box::Box() : StaticMeshObject(STATICMESH_MODEL::BOX)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\boxCollision.obj");
	AddBoxCollisionComponent(COLLISION_TAG::OBJECT);
}

Box::Box(const XMFLOAT3& position, const XMFLOAT3& scale)
	:Box()
{
	m_Position = position;
	m_Scale = scale;
}

Box::Box(const XMFLOAT3& position, const XMFLOAT3& scale, const XMFLOAT3& rotate) 
	: Box(position,scale)
{
	m_Rotation = rotate;
}
