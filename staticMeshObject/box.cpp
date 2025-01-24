#include "box.h"
#include "manager/objModelManager.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"

void Box::MoveControl(const float& deltaTime)
{
	// èÉêàâºëzä÷êîÇÃà◊âºé¿ëï
}

Box::Box()
	: StaticMeshObject(STATICMESH_MODEL::BOX)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\box.obj");
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::OBJECT);
	AddComponent<ShaderComponent>(this);
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
