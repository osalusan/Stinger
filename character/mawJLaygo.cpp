#include "mawJLaygo.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "manager/fbxModelManager.h"
#include "behaviorTree/behaviorTree.h"
#include "meshFiled/meshFiled.h"
#include "renderer/fbxModelRenderer.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };

// �u�����h���x
constexpr float DEFAULT_BLEND_VALUE_MAWJ = 4.0f;
// �����蔻��̃X�P�[������
constexpr XMFLOAT3 TEST_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.0f,DEFAULT_SCALE_MAWJ.z * 1.0f };
constexpr XMFLOAT3 HIP_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.35f,DEFAULT_SCALE_MAWJ.y * 0.7f,DEFAULT_SCALE_MAWJ.z * 2.4f };
constexpr XMFLOAT3 SPINE_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.3f,DEFAULT_SCALE_MAWJ.y * 0.8f,DEFAULT_SCALE_MAWJ.z * 2.3f };
constexpr XMFLOAT3 NECK_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.2f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 HAND_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.125f,DEFAULT_SCALE_MAWJ.y * 1.25f,DEFAULT_SCALE_MAWJ.z * 1.425f };
constexpr XMFLOAT3 ARM_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.15f,DEFAULT_SCALE_MAWJ.y * 1.1f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 ARMFORE_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.15f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 SHOULDER_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.12f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.5f };
constexpr XMFLOAT3 FOOT_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 0.9f,DEFAULT_SCALE_MAWJ.y * 1.1f,DEFAULT_SCALE_MAWJ.z * 1.2f };
constexpr XMFLOAT3 LEG_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 0.9f,DEFAULT_SCALE_MAWJ.y * 1.4f,DEFAULT_SCALE_MAWJ.z * 1.3f };
constexpr XMFLOAT3 UPLEG_COLL_SCALE_MAWJ = { DEFAULT_SCALE_MAWJ.x * 1.0f,DEFAULT_SCALE_MAWJ.y * 1.3f,DEFAULT_SCALE_MAWJ.z * 1.4f };

constexpr const char* HIP_NAME_MAWJ = "mixamorig:Hips";
constexpr const char* SPINE_NAME_MAWJ = "mixamorig:Spine";
constexpr const char* SPINE1_NAME_MAWJ = "mixamorig:Spine1";
constexpr const char* SPINE2_NAME_MAWJ = "mixamorig:Spine2";
constexpr const char* NECK_NAME_MAWJ = "mixamorig:Neck";
constexpr const char* RIGHTHAND_NAME_MAWJ = "mixamorig:RightHand";
constexpr const char* LEFTHAND_NAME_MAWJ = "mixamorig:LeftHand";
constexpr const char* RIGHT_ARM_NAME_MAWJ = "mixamorig:RightArm";
constexpr const char* LEFT_ARM_NAME_MAWJ = "mixamorig:LeftArm";
constexpr const char* RIGHT_FOREARM_NAME_MAWJ = "mixamorig:RightForeArm";
constexpr const char* LEFT_FOREARM_NAME_MAWJ = "mixamorig:LeftForeArm";
constexpr const char* RIGHT_SHOULDER_NAME_MAWJ = "mixamorig:RightShoulder";
constexpr const char* LEFT_SHOULDER_NAME_MAWJ = "mixamorig:LeftShoulder";
constexpr const char* RIGHT_FOOT_NAME_MAWJ = "mixamorig:RightFoot";
constexpr const char* LEFT_FOOT_NAME_MAWJ = "mixamorig:LeftFoot";
constexpr const char* RIGHT_LEG_NAME_MAWJ = "mixamorig:RightLeg";
constexpr const char* LEFT_LEG_NAME_MAWJ = "mixamorig:LeftLeg";
constexpr const char* RIGHT_UPLEG_NAME_MAWJ = "mixamorig:RightUpLeg";
constexpr const char* LEFT_UPLEG_NAME_MAWJ = "mixamorig:LeftUpLeg";


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
			const XMMATRIX& modelScale = XMMatrixScaling(1.0f / boxCustomScl.x, 1.0f / boxCustomScl.y, 1.0f / boxCustomScl.z); // ���f���ɃX�P�[�������킹��
			const XMMATRIX& matrixScale = XMMatrixScaling(boxCustomScl.x, boxCustomScl.y, boxCustomScl.z);
			XMMATRIX boneWorldMatrix = modelScale * boneMatrix * matrixScale * world;

			boneWorldMatrix.r[3].m128_f32[0] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[1] = 0.0f;
			boneWorldMatrix.r[3].m128_f32[2] = 0.0f;

			XMFLOAT3 offsetPos = {};
			XMFLOAT3 collisionScl = {};

			// ���ʂ��ƂɕύX
			if (boxName == HIP_NAME_MAWJ)
			{
				collisionScl = HIP_COLL_SCALE_MAWJ;
			}			
			else if (boxName == SPINE_NAME_MAWJ || boxName == SPINE1_NAME_MAWJ || boxName == SPINE2_NAME_MAWJ)
			{
				collisionScl = SPINE_COLL_SCALE_MAWJ;
			}
			else if (boxName == NECK_NAME_MAWJ)
			{
				collisionScl = NECK_COLL_SCALE_MAWJ;
			}			
			else if (boxName == RIGHTHAND_NAME_MAWJ || boxName == LEFTHAND_NAME_MAWJ)
			{
				collisionScl = HAND_COLL_SCALE_MAWJ;
			}
			else if (boxName == RIGHT_ARM_NAME_MAWJ || boxName == LEFT_ARM_NAME_MAWJ)
			{
				collisionScl = ARM_COLL_SCALE_MAWJ;
			}			
			else if (boxName == RIGHT_FOREARM_NAME_MAWJ || boxName == LEFT_FOREARM_NAME_MAWJ)
			{
				collisionScl = ARMFORE_COLL_SCALE_MAWJ;
			}
			else if (boxName == RIGHT_SHOULDER_NAME_MAWJ || boxName == LEFT_SHOULDER_NAME_MAWJ)
			{
				collisionScl = SHOULDER_COLL_SCALE_MAWJ;
			}			
			else if (boxName == RIGHT_FOOT_NAME_MAWJ || boxName == LEFT_FOOT_NAME_MAWJ)
			{
				collisionScl = FOOT_COLL_SCALE_MAWJ;
			}			
			else if (boxName == RIGHT_LEG_NAME_MAWJ || boxName == LEFT_LEG_NAME_MAWJ)
			{
				collisionScl = LEG_COLL_SCALE_MAWJ;
			}			
			else if (boxName == RIGHT_UPLEG_NAME_MAWJ || boxName == LEFT_UPLEG_NAME_MAWJ)
			{
				collisionScl = UPLEG_COLL_SCALE_MAWJ;
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
	// �������z�֐��̈׋����
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
	// �r�w�C�r�A�c���[�̍쐬����ɌĂ�
	EnemyDataLoadCSV("asset\\csv\\mawJLaygo.csv");
	CreateTree();

	ReservModel(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\mawJ\\mawJLaygo.fbx");
	if (m_Scale.y == 1.0f)
	{
		m_Scale = DEFAULT_SCALE_MAWJ;
	}

	// �p�����[�^�ݒ�
	m_EnableGravity = true;
	m_BlendTimeValue = DEFAULT_BLEND_VALUE_MAWJ;

	// ��
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, HIP_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE1_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, SPINE2_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, NECK_NAME_MAWJ));
	// �r
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHTHAND_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFTHAND_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_ARM_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_ARM_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_FOREARM_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_FOREARM_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_SHOULDER_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_SHOULDER_NAME_MAWJ));
	// ��
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_FOOT_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_FOOT_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_LEG_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_LEG_NAME_MAWJ));	
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, RIGHT_UPLEG_NAME_MAWJ));
	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS, LEFT_UPLEG_NAME_MAWJ));
	
	
	// �����蔻��̌�ɒǉ�
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}