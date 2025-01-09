#include "transparentWall.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale)
	: StaticMeshObject(STATICMESH_MODEL::BOX)
{
	m_Position = pos;
	m_Scale = scale;
	ObjModelManager::ReservModel(m_Model, "asset\\model\\box.obj");
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this);
	m_Visible = false;

}
