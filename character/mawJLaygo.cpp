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
constexpr XMFLOAT3 HAND_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 0.1f,DEFAULT_SCALE_MAWJ.y * 0.12f,DEFAULT_SCALE_MAWJ.z * 0.12f };
constexpr XMFLOAT3 HIP_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 0.15f,DEFAULT_SCALE_MAWJ.y * 0.2f,DEFAULT_SCALE_MAWJ.z * 0.3f };

constexpr const char* HIP_NAME_MAWJ = "mixamorig:Hips";
constexpr const char* RIGHTHAND_NAME_MAWJ = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME_MAWJ = "mixamorig:LeftHand";

// ----------------------- private -----------------------
void MawJLaygo::CustomCollisionInfo()
{
	if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		if (model == nullptr) return;

		for (BoxCollisionComponent* boxColli : m_BoxCollisionCaches)
		{
			if (boxColli == nullptr) continue;

			for (const std::pair<std::string, BONE>& bonePair : model->GetBone())
			{
				if (bonePair.first != boxColli->GetName()) continue;
				if (m_CollOffsetPos.count(bonePair.first) <= 0) continue;

				const XMFLOAT3& customPos = { 
					m_Position.x + m_CollOffsetPos[bonePair.first].x,
					m_Position.y + m_CollOffsetPos[bonePair.first].y,
					m_Position.z + m_CollOffsetPos[bonePair.first].z };
				boxColli->SetCollisionInfo(customPos, model->GetScale(), GetRotationMatrix(), bonePair.second.WorldMatrix);
			}
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

	for (BoxCollisionComponent* box : m_BoxCollisionCaches)
	{
		if (box == nullptr) continue;

		XMFLOAT3 offsetPos = {};
		// 部位ごとに追加していく
		if (box->GetName() == HIP_NAME_MAWJ)
		{
			box->SetScale(HIP_COLL_SCALE_MAWJ);
			offsetPos.x = 0.0f;
			offsetPos.y = 0.0f;
			offsetPos.z = 0.0f;
			m_CollOffsetPos.emplace(HIP_NAME_MAWJ, offsetPos);
		}
		if (box->GetName() == RIGHTHAND_NAME_MAWJ)
		{
			box->SetScale(HAND_COLL_SCALE_MAWJ);
			offsetPos.x = 0.0f;
			offsetPos.y = 0.0f;
			offsetPos.z = 0.0f;
			m_CollOffsetPos.emplace(RIGHTHAND_NAME_MAWJ, offsetPos);
		}
		if (box->GetName() == LEFTHAND_NAME_MAWJ)
		{
			box->SetScale(HAND_COLL_SCALE_MAWJ);
			offsetPos.x = 0.0f;
			offsetPos.y = 0.0f;
			offsetPos.z = 0.0f;
			m_CollOffsetPos.emplace(LEFTHAND_NAME_MAWJ, offsetPos);
		}
	}

	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}