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
constexpr XMFLOAT3 TEST_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.0f,DEFAULT_SCALE_MAWJ.z * 1.0f };
constexpr XMFLOAT3 HAND_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.125f,DEFAULT_SCALE_MAWJ.y * 1.13f,DEFAULT_SCALE_MAWJ.z * 1.125f };
constexpr XMFLOAT3 HIP_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.65f,DEFAULT_SCALE_MAWJ.y * 2.4f,DEFAULT_SCALE_MAWJ.z * 2.5f };

constexpr const char* HIP_NAME_MAWJ = "mixamorig:Hips";
constexpr const char* RIGHTHAND_NAME_MAWJ = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME_MAWJ = "mixamorig:LeftHand";
constexpr const char* RIGHT_ARM = "mixamorig:RightArm";
constexpr const char* LEFT_ARM = "mixamorig:LeftArm";

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
		const XMFLOAT3& boxScale = boxColli->GetScale();
		const XMFLOAT3& boxCustomScl = { m_Scale.x - boxScale.x, m_Scale.y - boxScale.y, m_Scale.z - boxScale.z};

		const XMMATRIX& scl = XMMatrixScaling((m_Scale.x * modelScale.x) * 0.5f, (m_Scale.y * modelScale.y) * 0.5f, (m_Scale.z * modelScale.z) * 0.5f);
		const XMMATRIX& rot = boxColli->GetBoneMatrix();
		const XMMATRIX& trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		world = scl * rot * trans;

		for (const std::pair<std::string, BONE>& bonePair : model->GetBone())
		{
			const std::string& boxName = boxColli->GetName();
			if (bonePair.first != boxName) continue;

			const XMMATRIX& boneMatrix = model->AiMatrixToXMMATRIX(bonePair.second.WorldMatrix);
			const XMMATRIX& modelScale = XMMatrixScaling(1.0f / boxCustomScl.x, 1.0f / boxCustomScl.y, 1.0f / boxCustomScl.z); // モデルにスケールを合わせる
			const XMMATRIX& matrixScale = XMMatrixScaling(boxCustomScl.x, boxCustomScl.y, boxCustomScl.z);
			XMMATRIX boneWorldMatrix = modelScale * boneMatrix * matrixScale * world;

			boneWorldMatrix.r[3].m128_f32[0] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[1] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[2] = 0.0f;

			XMFLOAT3 offsetPos = {};
			XMFLOAT3 collisionScl = {};

			// 部位ごとに変更
			if (boxName == HIP_NAME_MAWJ)
			{
				collisionScl = HIP_COLL_SCALE_MAWJ;
			}
			else if (boxName == RIGHTHAND_NAME_MAWJ)
			{
				collisionScl = HAND_COLL_SCALE_MAWJ;
			}
			else if (boxName == LEFTHAND_NAME_MAWJ)
			{
				collisionScl = HAND_COLL_SCALE_MAWJ;
			}
			else
			{
				collisionScl = TEST_COLL_SCALE_MAWJ;
			}

			boxColli->SetCollisionInfo(offsetPos, collisionScl,model->GetCenter(), model->GetScale(), GetRotationMatrix(), boneMatrix);
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

	//m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, HIP_NAME_MAWJ));
	//m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHTHAND_NAME_MAWJ));
	//m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFTHAND_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_ARM));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_ARM));
	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}