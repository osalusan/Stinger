#include "ruins.h"
#include "manager/objModelManager.h"
#include "component/shaderComponent.h"

void Ruins::MoveControl(const float& deltaTime)
{
	// ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼À‘•
}

Ruins::Ruins(const XMFLOAT3& position, const XMFLOAT3& scale, const XMFLOAT3& rotate)
	: StaticMeshObject(STATICMESH_MODEL::RUINS)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\box.obj");
	AddComponent<ShaderComponent>(this);
}
