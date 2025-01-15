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
constexpr XMFLOAT3 HAND_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 0.1f,DEFAULT_SCALE_MAWJ.y * 0.1f,DEFAULT_SCALE_MAWJ.z * 0.1f };

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


	// パーツの名前
	const std::string& hip = "mixamorig:Hips";
	const std::string& rightHand = "mixamorig:RightHand";
	const std::string& leftHand = "mixamorig:LeftHand";

	// m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:Hips"));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:RightHand"));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, "mixamorig:LeftHand"));

	for (BoxCollisionComponent* box : m_BoxCollisionCaches)
	{
		if (box == nullptr) continue;

		// 部位ごとに追加していく
		if (box->GetName() == hip)
		{
			box->SetScale({ m_Scale.x * 0.5f,m_Scale.y * 0.8f ,m_Scale.z * 0.5f });
		}
		if (box->GetName() == rightHand)
		{
			box->SetScale(HAND_SCALE_MAWJ);
		}
		if (box->GetName() == leftHand)
		{
			box->SetScale(HAND_SCALE_MAWJ);
		}
	}

	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}