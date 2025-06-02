#include "mawJLaygo.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/scene.h"
#include "behaviorTree/behaviorTree.h"
#include "meshFiled/meshFiled.h"
#include "renderer/fbxModelRenderer.h"
#include "character/player.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };

// ブレンド速度
constexpr float DEFAULT_BLEND_VALUE_MAWJ = 4.0f;
// 当たり判定のスケール調整
constexpr XMFLOAT3 TEST_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.0f,DEFAULT_SCALE_MAWJ.z * 1.0f };
constexpr XMFLOAT3 HIP_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.35f,DEFAULT_SCALE_MAWJ.y * 0.9f,DEFAULT_SCALE_MAWJ.z * 2.5f };
constexpr XMFLOAT3 SPINE_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.34f,DEFAULT_SCALE_MAWJ.y * 0.8f,DEFAULT_SCALE_MAWJ.z * 2.4f };
constexpr XMFLOAT3 NECK_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.2f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 HAND_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.125f,DEFAULT_SCALE_MAWJ.y * 1.25f,DEFAULT_SCALE_MAWJ.z * 1.425f };
constexpr XMFLOAT3 ARM_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.15f,DEFAULT_SCALE_MAWJ.y * 1.1f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 ARMFORE_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.15f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 SHOULDER_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.12f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 FOOT_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 0.9f,DEFAULT_SCALE_MAWJ.y * 1.1f,DEFAULT_SCALE_MAWJ.z * 1.2f };
constexpr XMFLOAT3 LEG_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 0.9f,DEFAULT_SCALE_MAWJ.y * 1.4f,DEFAULT_SCALE_MAWJ.z * 1.35f };
constexpr XMFLOAT3 UPLEG_COLL_SCALE = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.4f };

constexpr const char* HIP_NAME = "mixamorig:Hips";
constexpr const char* SPINE_NAME = "mixamorig:Spine";
constexpr const char* SPINE1_NAME = "mixamorig:Spine1";
constexpr const char* SPINE2_NAME = "mixamorig:Spine2";
constexpr const char* NECK_NAME = "mixamorig:Neck";
constexpr const char* RIGHTHAND_NAME = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME = "mixamorig:LeftHand";
constexpr const char* RIGHT_ARM_NAME = "mixamorig:RightArm";
constexpr const char* LEFT_ARM_NAME = "mixamorig:LeftArm";
constexpr const char* RIGHT_FOREARM_NAME = "mixamorig:RightForeArm";
constexpr const char* LEFT_FOREARM_NAME = "mixamorig:LeftForeArm";
constexpr const char* RIGHT_SHOULDER_NAME = "mixamorig:RightShoulder";
constexpr const char* LEFT_SHOULDER_NAME = "mixamorig:LeftShoulder";
constexpr const char* RIGHT_FOOT_NAME = "mixamorig:RightFoot";
constexpr const char* LEFT_FOOT_NAME = "mixamorig:LeftFoot";
constexpr const char* RIGHT_LEG_NAME = "mixamorig:RightLeg";
constexpr const char* LEFT_LEG_NAME = "mixamorig:LeftLeg";
constexpr const char* RIGHT_UPLEG_NAME = "mixamorig:RightUpLeg";
constexpr const char* LEFT_UPLEG_NAME = "mixamorig:LeftUpLeg";


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

			const XMMATRIX& boneMatrix = model->AiMatrixToXMMATRIX(bonePair.second.LocalMatrix);

			XMFLOAT3 offsetPos = {};
			XMFLOAT3 collisionScl = {};

			// 部位ごとに変更
			if (boxName == HIP_NAME)
			{
				collisionScl = HIP_COLL_SCALE;
			}			
			else if (boxName == SPINE_NAME || boxName == SPINE1_NAME || boxName == SPINE2_NAME)
			{
				collisionScl = SPINE_COLL_SCALE;
			}
			else if (boxName == NECK_NAME)
			{
				collisionScl = NECK_COLL_SCALE;
			}			
			else if (boxName == RIGHTHAND_NAME || boxName == LEFTHAND_NAME)
			{
				collisionScl = HAND_COLL_SCALE;
			}
			else if (boxName == RIGHT_ARM_NAME || boxName == LEFT_ARM_NAME)
			{
				collisionScl = ARM_COLL_SCALE;
			}			
			else if (boxName == RIGHT_FOREARM_NAME || boxName == LEFT_FOREARM_NAME)
			{
				collisionScl = ARMFORE_COLL_SCALE;
			}
			else if (boxName == RIGHT_SHOULDER_NAME || boxName == LEFT_SHOULDER_NAME)
			{
				collisionScl = SHOULDER_COLL_SCALE;
			}			
			else if (boxName == RIGHT_FOOT_NAME || boxName == LEFT_FOOT_NAME)
			{
				collisionScl = FOOT_COLL_SCALE;
			}			
			else if (boxName == RIGHT_LEG_NAME || boxName == LEFT_LEG_NAME)
			{
				collisionScl = LEG_COLL_SCALE;
			}			
			else if (boxName == RIGHT_UPLEG_NAME || boxName == LEFT_UPLEG_NAME)
			{
				collisionScl = UPLEG_COLL_SCALE;
			}
			else
			{
				collisionScl = TEST_COLL_SCALE;
			}

			boxColli->SetCollisionInfo(offsetPos, collisionScl,model->GetCenter(), model->GetScale(), GetRotationMatrix(), boneMatrix);
		}
	}

}

void MawJLaygo::CollisionControl()
{
	float groundHeight = 0.0f;
	if (m_MeshFiledCache != nullptr)
	{
		groundHeight = m_MeshFiledCache->GetHeight(m_Position);
	}

	// パリィされた時は強制的に地面に移動させる
	if (m_ParryRecoil)
	{
		m_Position.y = groundHeight;
	}

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
		ResetGravity();
	}

	// ダメージ計算
	if (m_AttackDamage != 0.0f)
	{
		for (BoxCollisionComponent* boxColl : m_BoxCollisionCaches)
		{
			if (boxColl == nullptr) continue;
			if (m_CurrentAttackParts == ATTACK_PARTS::NONE || m_CurrentAttackParts == ATTACK_PARTS::MAX) break;

			if (m_CurrentAttackParts != ATTACK_PARTS::ALL)
			{
				const std::string& collName = boxColl->GetName();
				if (m_PartsCategory.count(collName) <= 0) continue;
				const ATTACK_PARTS& partsName = m_PartsCategory.at(collName);

				if (m_CurrentAttackParts != partsName)
				{
					
				} 
				// 手の判定 / 右手や左手は手としても判定される
				else if (m_CurrentAttackParts == ATTACK_PARTS::ARM && partsName == ATTACK_PARTS::RIGHT_ARM || partsName == ATTACK_PARTS::LEFT_ARM)
				{
					
				}
				else
				{
					continue;
				}
			}

			if (m_PlayerCache == nullptr)
			{
				Scene* scene = SceneManager::GetScene();
				if (scene == nullptr) break;
				ObjectManager* objectManager = scene->GetObjectManager();
				if (objectManager == nullptr) break;
				m_PlayerCache = objectManager->GetPlayer();
			}

			if (m_PlayerCache == nullptr) break;

			if (m_AttackAccept) break;

			// 盾に当たっているか確認
			if (boxColl->CheckHitObject(COLLISION_TAG::EQUIPMENT_OBJECT))
			{
				m_PlayerCache->HitShiled(boxColl);
			}

			// ダメージを与える
			if (!boxColl->CheckHitObject(OBJECT::PLAYER)) continue;			

			// パリィ可能か不可能かで、呼ぶ関数を変更
			if (m_ParryPossibleAtk)
			{
				m_PlayerCache->TakeDamageParryPossible(m_AttackDamage);
			}
			else
			{
				m_PlayerCache->TakeDamage(m_AttackDamage);
			}
			
			// 攻撃成功
			m_AttackAccept = true;
			break;
		}
	}


	// デバッグ時に文字を表示させたいので、ダメージ計算後に呼ぶ処理を、BossEnemyのMoveControlの一番最初に配置
	// ※IMGUIのデバッグウインドウ用

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

	// 体
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, HIP_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE1_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE2_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, NECK_NAME));

	m_PartsCategory.emplace(HIP_NAME,ATTACK_PARTS::BODY);
	m_PartsCategory.emplace(SPINE_NAME,ATTACK_PARTS::BODY);
	m_PartsCategory.emplace(SPINE1_NAME,ATTACK_PARTS::BODY);
	m_PartsCategory.emplace(SPINE2_NAME,ATTACK_PARTS::BODY);
	m_PartsCategory.emplace(NECK_NAME,ATTACK_PARTS::BODY);
	// 腕
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHTHAND_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFTHAND_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_ARM_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_ARM_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_FOREARM_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_FOREARM_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_SHOULDER_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_SHOULDER_NAME));

	m_PartsCategory.emplace(RIGHTHAND_NAME, ATTACK_PARTS::RIGHT_ARM);
	m_PartsCategory.emplace(LEFTHAND_NAME, ATTACK_PARTS::LEFT_ARM);
	m_PartsCategory.emplace(RIGHT_ARM_NAME, ATTACK_PARTS::RIGHT_ARM);
	m_PartsCategory.emplace(LEFT_ARM_NAME, ATTACK_PARTS::LEFT_ARM);
	m_PartsCategory.emplace(RIGHT_FOREARM_NAME, ATTACK_PARTS::RIGHT_ARM);
	m_PartsCategory.emplace(LEFT_FOREARM_NAME, ATTACK_PARTS::LEFT_ARM);
	m_PartsCategory.emplace(RIGHT_SHOULDER_NAME, ATTACK_PARTS::RIGHT_ARM);
	m_PartsCategory.emplace(LEFT_SHOULDER_NAME, ATTACK_PARTS::LEFT_ARM);

	// 足
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_FOOT_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_FOOT_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_LEG_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_LEG_NAME));	
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_UPLEG_NAME));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_UPLEG_NAME));
	
	m_PartsCategory.emplace(RIGHT_FOOT_NAME, ATTACK_PARTS::RIGHT_LEG);
	m_PartsCategory.emplace(LEFT_FOOT_NAME, ATTACK_PARTS::LEFT_LEG);
	m_PartsCategory.emplace(RIGHT_LEG_NAME, ATTACK_PARTS::RIGHT_LEG);
	m_PartsCategory.emplace(LEFT_LEG_NAME, ATTACK_PARTS::LEFT_LEG);
	m_PartsCategory.emplace(RIGHT_UPLEG_NAME, ATTACK_PARTS::RIGHT_LEG);
	m_PartsCategory.emplace(LEFT_UPLEG_NAME, ATTACK_PARTS::LEFT_LEG);
	
	// 当たり判定の後に追加
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}