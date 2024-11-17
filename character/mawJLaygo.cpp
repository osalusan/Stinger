#include "mawJLaygo.h"
#include "manager/fbxModelManager.h"
#include "component/boxCollisionComponent.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorTree/selectorNode .h"
#include "behaviorNodes/DashAtThePlayerNode.h"

constexpr XMFLOAT3 MAWJ_DEFAULT_SCALE = { 0.1f,0.1f,0.1f };
constexpr float GRAVITY = 1200.0f;

// ----------------------- private -----------------------
void MawJLaygo::CustomCollisionInfo()
{
	m_ColliPosition = m_Position;
	m_ColliRotation = m_Rotation;
	m_ColliScale.x = m_Scale.x * 0.5f;
	m_ColliScale.y = m_Scale.y;
	m_ColliScale.z = m_Scale.z * 0.5f;
}

void MawJLaygo::CollisionControl()
{
	float groundHeight = 0.0f;

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
}

void MawJLaygo::CreateBehaviourTree()
{
	if (m_Tree == nullptr) return;

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	rootNode->AddChild<DashAtThePlayerNode>();
	m_Tree->CreateRoot(rootNode);
}


// ----------------------- public -----------------------

MawJLaygo::MawJLaygo(const XMFLOAT3& pos)
{
	ReservModel(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\MawJLaygo.fbx");
	m_Position = pos;
}


MawJLaygo::~MawJLaygo()
{

}

void MawJLaygo::Init()
{
	BossEnemy::Init();
	m_Scale = MAWJ_DEFAULT_SCALE;
	m_EnableGravity = true;
	m_GravityValue = GRAVITY;

	AddBoxCollisionComponent(COLLISION_TAG::PLAYER);
}