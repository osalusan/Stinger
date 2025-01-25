#include "equipmentObject.h"
#include "manager/objModelManager.h"
#include "manager/fbxModelManager.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "renderer/fbxModelRenderer.h"

void EquipmentObject::MoveControl(const float& deltaTime)
{
	if (m_ParentObject == nullptr) return;
	const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_ParentModel);
	if (model == nullptr) return;

	XMMATRIX boneMatrix = {};
	for (const std::pair<std::string, BONE>& bonePair : model->GetBone())
	{
		if (bonePair.first != m_BoneName) continue;

		boneMatrix = model->AiMatrixToXMMATRIX(bonePair.second.WorldMatrix);

		break;
	}

	XMMATRIX world, scl, rot, trans;
	XMMATRIX parentWorld, parentScl, parentRot, parentTrans;

	const XMFLOAT3& parentScale = m_ParentObject->GetScale();
	const XMFLOAT3& parentPos = m_ParentObject->GetPos();

	parentScl = XMMatrixScaling(parentScale.x, parentScale.y, parentScale.z);
	parentRot = m_ParentObject->GetRotationMatrix();
	parentTrans = XMMatrixTranslation(parentPos.x, parentPos.y, parentPos.z);

	parentWorld = parentScl * parentRot * parentTrans;

	scl = XMMatrixScaling((1.0f / parentScale.x) * m_OffsetScale.x, (1.0f / parentScale.y) * m_OffsetScale.y, (1.0f / parentScale.z) * m_OffsetScale.z);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	trans = XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);

	world = rot * trans * scl * boneMatrix * parentWorld;

	XMVECTOR decScl, decRot, decTrans = {};
	XMMatrixDecompose(&decScl, &decRot, &decTrans, world);

	m_Position.x = decTrans.m128_f32[0];
	m_Position.y = decTrans.m128_f32[1];
	m_Position.z = decTrans.m128_f32[2];

	m_Scale.x = decScl.m128_f32[0];
	m_Scale.y = decScl.m128_f32[1];
	m_Scale.z = decScl.m128_f32[2];

	world.r[3].m128_f32[0] = 0.0f;
	world.r[3].m128_f32[1] = 0.0f;
	world.r[3].m128_f32[2] = 0.0f;

	m_RotationMatrix = world;

	m_ShaderCache->SetRotMatrix(m_RotationMatrix);
	//XMFLOAT4 fquat;
	//XMStoreFloat4(&fquat, decRot);

	//float x = fquat.x;
	//float y = fquat.y;
	//float z = fquat.z;
	//float w = fquat.w;

	//// �����ł́uYaw-Pitch-Roll�v�̏�(����n)��z�肵�����̗�
	//float pitch = asinf(2.0f * (w * y - x * z));
	//float yaw = atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
	//float roll = atan2f(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

	//const float& x = decRot.m128_f32[0];
	//const float& y = decRot.m128_f32[1];
	//const float& z = decRot.m128_f32[2];
	//const float& w = decRot.m128_f32[3];

	//float pitch = asinf(2.0f * (w * y - x * z));
	//float yaw  = atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
	//float roll = atan2f(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

	//float pitch = asinf(-world.r[2].m128_f32[1]);
	//float yaw = atan2f(-world.r[2].m128_f32[0], -world.r[2].m128_f32[2]);
	//float roll = atan2f(-world.r[0].m128_f32[1], -world.r[1].m128_f32[1]);


	//m_Rotation.x = roll;
	//m_Rotation.y = pitch;
	//m_Rotation.x = yaw;
}

EquipmentObject::~EquipmentObject()
{
	m_ShaderCache = nullptr;
}

EquipmentObject::EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel, const std::string& fileName, const ANIMETION_MODEL& parentModel, const char* boneName, const XMFLOAT3& scale)
	: StaticMeshObject(staticModel)
{
	if (m_ParentObject == nullptr)
	{
		m_ParentObject = gameObj;
	}

	ObjModelManager::ReservModel(m_Model, fileName);
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::EQUIPMENT_OBJECT);
	m_ShaderCache = AddComponent<ShaderComponent>(this);

	m_ParentModel = parentModel;
	m_BoneName = boneName;
	m_OffsetScale = scale;
}

EquipmentObject::EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel, const std::string& fileName, const ANIMETION_MODEL& parentModel, const char* boneName, const XMFLOAT3& scale, const XMFLOAT3& pos)
	:EquipmentObject(gameObj,staticModel,fileName,parentModel,boneName,scale)
{
	m_OffsetPos = pos;
}