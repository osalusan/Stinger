#include "mawJLaygo.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "manager/fbxModelManager.h"
#include "behaviorTree/behaviorTree.h"
#include "meshFiled/meshFiled.h"
#include "renderer/fbxModelRenderer.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };

// ブレンド速度
constexpr float DEFAULT_BLEND_VALUE_MAWJ = 4.0f;
constexpr XMFLOAT3 HAND_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.125f,DEFAULT_SCALE_MAWJ.y * 1.13f,DEFAULT_SCALE_MAWJ.z * 1.125f };
constexpr XMFLOAT3 HIP_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.65f,DEFAULT_SCALE_MAWJ.y * 2.4f,DEFAULT_SCALE_MAWJ.z * 2.5f };

constexpr const char* HIP_NAME_MAWJ = "mixamorig:Hips";
constexpr const char* RIGHTHAND_NAME_MAWJ = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME_MAWJ = "mixamorig:LeftHand";

// ----------------------- private -----------------------
void MawJLaygo::CustomCollisionInfo()
{
	const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model);

	if (model == nullptr) return;

	for (BoxCollisionComponent* boxColli : m_BoxCollisionCaches)
	{
		if (boxColli == nullptr) continue;

		XMMATRIX world;
		const XMFLOAT3& modelScale = boxColli->GetModelScale();
		const XMFLOAT3& customScale = { m_Scale.x - boxColli->GetScale().x, m_Scale.y - boxColli->GetScale().y, m_Scale.z - boxColli->GetScale().z};

		const XMMATRIX& scl = XMMatrixScaling((m_Scale.x * modelScale.x) * 0.5f, (m_Scale.y * modelScale.y) * 0.5f, (m_Scale.z * modelScale.z) * 0.5f);
		const XMMATRIX& rot = boxColli->GetBoneMatrix();
		const XMMATRIX& trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		world = scl * rot * trans;

		for (const std::pair<std::string, BONE>& bonePair : model->GetBone())
		{
			if (bonePair.first != boxColli->GetName()) continue;

			BoneOffset(bonePair.second);
			const XMMATRIX& modelScale = XMMatrixScaling(1.0f / customScale.x, 1.0f / customScale.y, 1.0f / customScale.z); // モデルにスケールを合わせる
			const XMMATRIX& matrixScale = XMMatrixScaling(customScale.x, customScale.y, customScale.z);
			XMMATRIX boneWorldMatrix = modelScale * bonePair.second.WorldMatrix * matrixScale * world;

			boneWorldMatrix.r[3].m128_f32[0] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[1] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[2] = 0.0f;

			XMFLOAT3 up;
			XMStoreFloat3(&up, boneWorldMatrix.r[1]);

			const XMFLOAT3& modelRight = GetRight();

			XMFLOAT3 offsetPos = {};
			XMFLOAT3 customScl = {};

			float handOffsetRight = -1.8f;

			//if (boxColli->GetName() == HIP_NAME_MAWJ)
			//{
			//	const float& offset = -0.95f;
			//	offsetPos.y = up.y * offset;
			//}
			//if (boxColli->GetName() == RIGHTHAND_NAME_MAWJ)
			//{
			//	offsetPos.x = modelRight.x * handOffsetRight;
			//	offsetPos.y = modelRight.y * handOffsetRight;
			//	offsetPos.z = modelRight.z * handOffsetRight;
			//}
			//if (boxColli->GetName() == LEFTHAND_NAME_MAWJ)
			//{
			//	offsetPos.x = modelRight.x * -handOffsetRight;
			//	offsetPos.y = modelRight.y * -handOffsetRight;
			//	offsetPos.z = modelRight.z * -handOffsetRight;
			//}
			if (boxColli->GetName() == HIP_NAME_MAWJ)
			{
				customScl = HIP_COLL_SCALE_MAWJ;
			}
			if (boxColli->GetName() == RIGHTHAND_NAME_MAWJ)
			{
				customScl = HAND_COLL_SCALE_MAWJ;
			}
			if (boxColli->GetName() == LEFTHAND_NAME_MAWJ)
			{
				customScl = HAND_COLL_SCALE_MAWJ;
			}

			const XMFLOAT3& customPos = {
				offsetPos.x,
				offsetPos.y,
				offsetPos.z };
			boxColli->SetCollisionInfo(customPos, customScl,model->GetCenter(), model->GetScale(), GetRotationMatrix(), bonePair.second.WorldMatrix);
		}
	}

}

void MawJLaygo::CollisionControl()
{

	float groundHeight = 0.0f;
	if (m_MeshFiled != nullptr)
	{
		groundHeight = m_MeshFiled->GetHeight(m_Position);
	}

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
}

void MawJLaygo::AnimationControl()
{
	// 純粋仮想関数の為空実装
}

void MawJLaygo::BoneOffset(const BONE& bone)
{

}

// ----------------------- public -----------------------

MawJLaygo::~MawJLaygo()
{
	delete m_Tree;
	m_Tree = nullptr;
}

void MawJLaygo::Init()
{
	Character::Init();
	// ビヘイビアツリーの作成より先に呼ぶ
	EnemyDataLoadCSV("asset\\csv\\mawJLaygo.csv");
	CreateTree();

	ReservModel(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\mawJ\\mawJLaygo.fbx");
	if (m_Scale.y == 1.0f)
	{
		m_Scale = DEFAULT_SCALE_MAWJ;
	}

	// パラメータ設定
	m_EnableGravity = true;
	m_BlendTimeValue = DEFAULT_BLEND_VALUE_MAWJ;

	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, HIP_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHTHAND_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFTHAND_NAME_MAWJ));
	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}