#include "mawJLaygo.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "manager/fbxModelManager.h"
#include "behaviorTree/behaviorTree.h"
#include "meshFiled/meshFiled.h"
#include "renderer/fbxModelRenderer.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };
constexpr float GRAVITY = 1200.0f;
constexpr float MOVE_SPEED_MAWJ = 2000.0f;

constexpr int DEFAULT_HEALTH_MAWJ = 100;

// 攻撃範囲
constexpr float SHORT_RANGE_MAWJ = 30.0f;
constexpr float MIDDLE_RANGE_MAWJ = 70.0f;
constexpr float LONG_RANGE_MAWJ = 100.0f;

// ブレンド速度
constexpr float DEFAULT_BLEND_VALUE_MAWJ = 4.0f;

// 消費スタミナ
constexpr float MAX_STAMINA = 10.0f;

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

				boxColli->SetCollisionInfo(m_Position, model->GetScale(), GetRotationMatrix(), bonePair.second.WorldMatrix);
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
	BossEnemy::Init();
	ReservModel(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\mawJ\\mawJLaygo.fbx");
	if (m_Scale.y == 1.0f)
	{
		m_Scale = DEFAULT_SCALE_MAWJ;
	}

	EnemyDataLoadCSV("asset\\csv\\mawJLaygo.csv");

	// パラメータ設定
	m_EnableGravity = true;
	m_GravityValue = GRAVITY;
	m_Health = DEFAULT_HEALTH_MAWJ;
	m_MoveSpeed = MOVE_SPEED_MAWJ;
	m_ShortRange = SHORT_RANGE_MAWJ;
	m_MiddleRange = MIDDLE_RANGE_MAWJ;
	m_LongRange = LONG_RANGE_MAWJ;
	m_BlendTimeValue = DEFAULT_BLEND_VALUE_MAWJ;
	m_MaxStamina = MAX_STAMINA;
	m_StaminaValue = m_MaxStamina;

	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:Hips"));

	for (BoxCollisionComponent* box : m_BoxCollisionCaches)
	{
		if (box == nullptr) continue;

		// 部位ごとに追加していく
		if (box->GetName() == "mixamorig:Hips")
		{
			box->SetScale({ m_Scale.x * 0.5f,m_Scale.y * 0.8f ,m_Scale.z * 0.5f });
		}
	}

	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}