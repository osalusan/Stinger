#include "mawJLaygo.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorTree/selectorNode .h"
#include "taskNodes/dashAtThePlayerTask.h"
#include "taskNodes/checkRangeTask.h"
#include "taskNodes/rollAttackTask.h"
#include "taskNodes/animationAttackTask.h"

constexpr XMFLOAT3 DEFAULT_SCALE_MAWJ = { 0.1f,0.1f,0.1f };
constexpr float GRAVITY = 1200.0f;
constexpr float MOVE_SPEED_MAWJ = 2000.0f;

// 攻撃範囲
constexpr float SHORT_RANGE_MAWJ = 30.0f;
constexpr float MIDDLE_RANGE_MAWJ = 70.0f;
constexpr float LONG_RANGE_MAWJ = 100.0f;

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

	// playerのポインタを取得
	GameScene* scene = SceneManager::GetScene<GameScene>();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	Player* player = objectManager->GetPlayer();
	if (player == nullptr) return;

	// ビヘイビアツリーの作成

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	rootNode->AddChild<CheckRangeTask>(this, player);

	rootNode->AddChild<RollAttackTask>(this, player);

	rootNode->AddChild<AnimationAttackTask>(this, player);

	rootNode->AddChild<DashAtThePlayerTask>(this, player);

	// 一番最後に
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
	delete m_Tree;
	m_Tree = nullptr;
}

void MawJLaygo::Init()
{
	BossEnemy::Init();
	m_Scale = DEFAULT_SCALE_MAWJ;
	m_EnableGravity = true;
	m_GravityValue = GRAVITY;

	// パラメータ設定
	m_MoveSpeed = MOVE_SPEED_MAWJ;
	m_ShortRange = SHORT_RANGE_MAWJ;
	m_MiddleRange = MIDDLE_RANGE_MAWJ;
	m_LongRange = LONG_RANGE_MAWJ;

	AddBoxCollisionComponent(COLLISION_TAG::PLAYER);
}