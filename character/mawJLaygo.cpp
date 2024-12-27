#include "mawJLaygo.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "manager/fbxModelManager.h"
#include "behaviorTree/behaviorTree.h"
#include "meshFiled/meshFiled.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };
constexpr float GRAVITY = 1200.0f;
constexpr float MOVE_SPEED_MAWJ = 2000.0f;

constexpr int DEFAULT_HEALTH_MAWJ = 100;

// �U���͈�
constexpr float SHORT_RANGE_MAWJ = 30.0f;
constexpr float MIDDLE_RANGE_MAWJ = 70.0f;
constexpr float LONG_RANGE_MAWJ = 100.0f;

// �u�����h���x
constexpr float DEFAULT_BLEND_VALUE_MAWJ = 4.0f;

// ����X�^�~�i
constexpr float MAX_STAMINA = 10.0f;

// ----------------------- private -----------------------
void MawJLaygo::CustomCollisionInfo()
{
	for (BoxCollisionComponent* boxColli : m_BoxCollisionCaches)
	{
		if (boxColli == nullptr) continue;

		// TODO :�ύX�\�� / �f�o�b�O�p
		const XMFLOAT3& customScale = { m_Scale.x * 0.2f ,m_Scale.y ,m_Scale.z * 0.2f };
		boxColli->SetBoxCollisionInfo(m_Position, customScale, m_ModelCenter, m_ModelScale, GetRotationMatrix());
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
	BossEnemy::Init();
	ReservModel(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\mawJ\\MawJLaygo.fbx");
	if (m_Scale.y == 1.0f)
	{
		m_Scale = DEFAULT_SCALE_MAWJ;
	}

	// �p�����[�^�ݒ�
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

	m_BoxCollisionCaches.emplace_back(AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_BOSS));
	// �����蔻��̌�ɒǉ�
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}