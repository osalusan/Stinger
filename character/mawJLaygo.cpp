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

				boxColli->SetCollisionInfo(m_Position, model->GetScale(),m_Rotation, GetRotationMatrix(), bonePair.second.WorldMatrix);
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

	// m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:Hips"));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:RightHand"));

	for (BoxCollisionComponent* box : m_BoxCollisionCaches)
	{
		if (box == nullptr) continue;

		// 部位ごとに追加していく
		if (box->GetName() == "mixamorig:Hips")
		{
			box->SetScale({ m_Scale.x * 0.5f,m_Scale.y * 0.8f ,m_Scale.z * 0.5f });
		}
		if (box->GetName() == "mixamorig:RightHand")
		{
			box->SetScale({ m_Scale.x * 0.3f,m_Scale.y * 0.3f ,m_Scale.z * 0.3f });
		}
	}

	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}