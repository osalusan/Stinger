#include "box.h"
#include "manager/objModelManager.h"

Box::Box(XMFLOAT3 position, XMFLOAT3 scale): StaticMeshObject(STATICMESH_MODEL::BOX)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\box.obj");
	m_Position = position;
	m_Scale = scale;
}

void Box::Update(const float& deltaTime)
{
	// ƒˆ‰¼‘zŠÖ”‚Ìˆ×‹óÀ‘•
}
