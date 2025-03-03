#include "playerCamera.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "scene/gameScene.h"

constexpr float LENGTH_DEFAULT = 35.0f;
constexpr float OFFSET_TARGET_POS_Y = LENGTH_DEFAULT * 0.35f;
constexpr float LENGTH_CUTIN = 4.5f;
constexpr float CUTIN_TARGET_POS_Y = LENGTH_CUTIN * 1.25f;
constexpr float LENGTH_EXTRATTACK = LENGTH_DEFAULT * 1.55f;

// マウスの設定
constexpr XMINT2 CENTER = { SCREEN_WIDTH / 2  , SCREEN_HEIGHT / 2 };
constexpr XMFLOAT2 MOUSE_SPEED = { 0.002f,0.002f };
constexpr int MOUSE_MOVE_SPACE = 100;

PlayerCamera::~PlayerCamera()
{
	m_PlayerCache = nullptr;
}

void PlayerCamera::Init()
{
	GameObject::Init();
	if (m_PlayerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objectManager = scene->GetObjectManager();
		if (objectManager == nullptr) return;
		
		m_PlayerCache = objectManager->GetPlayer();
	}
}

void PlayerCamera::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);
	if (m_PlayerCache == nullptr) return;

	if (m_BossCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objectManager = scene->GetObjectManager();
		if (objectManager == nullptr) return;

		m_BossCache = objectManager->GetBossEnemy();
	}

	// 演出終わった後変な向きを向かない用
	if (m_UseRendition)
	{
		POINT MousePos{};
		GetCursorPos(&MousePos);

		m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
		m_OldMousePos = m_MousePos;
	}

	if (!m_CutInMode && !m_UseRendition)
	{
		POINT MousePos{};
		GetCursorPos(&MousePos);

		m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));

		//マウスの横移動処理
		m_Rotation.y += (m_MousePos.x - m_OldMousePos.x) * MOUSE_SPEED.x;
		//マウスの縦移動処理
		m_Rotation.x -= (m_MousePos.y - m_OldMousePos.y) * MOUSE_SPEED.y;
		//マウスの上下制限
		// 下
		if (m_Rotation.x > 0.54f)
		{
			m_Rotation.x = 0.54f;
		}
		// 上
		if (m_Rotation.x < -1.14f)
		{
			m_Rotation.x = -1.14f;
		}

		m_Target = m_PlayerCache->GetPos();
		m_Target.y += OFFSET_TARGET_POS_Y;
		

		//カメラの移動処理

		const float& cosPitch = cosf(m_Rotation.x);
		const float& sinPitch = sinf(m_Rotation.x);
		const float& sinYaw = sinf(m_Rotation.y);
		const float& cosYaw = cosf(m_Rotation.y);

		const float& horizontalDist = LENGTH_DEFAULT * cosPitch;

		m_Position.x = m_Target.x - horizontalDist * sinYaw;
		m_Position.z = m_Target.z - horizontalDist * cosYaw;
		m_Position.y = m_Target.y - LENGTH_DEFAULT * sinPitch;

		m_OldMousePos = m_MousePos;
		m_Length = LENGTH_DEFAULT;

#if _DEBUG
		if (InputManager::GetKeyPress('R'))
		{
			SetCursorPos(CENTER.x, CENTER.y);
			GetCursorPos(&MousePos);
			m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
			m_OldMousePos = m_MousePos;
		}
#else
		if (m_MousePos.x <= MOUSE_MOVE_SPACE || m_MousePos.x >= CENTER.x - MOUSE_MOVE_SPACE ||
			m_MousePos.y <= MOUSE_MOVE_SPACE || m_MousePos.y >= CENTER.y - MOUSE_MOVE_SPACE)
		{
			if (GetWindowActive())
			{
				SetCursorPos(CENTER.x, CENTER.y);
				GetCursorPos(&MousePos);
				m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
				m_OldMousePos = m_MousePos;
			}
		}
#endif // _DEBUG
	}
	// カットインのカメラ演出
	else
	{
		m_Length = LENGTH_CUTIN;

		if (m_BossCache == nullptr) return;

		if (m_UseRendition)
		{
			if (m_PlayerCache == nullptr || m_RendLength == 0.0f) return;

			const XMFLOAT3& enemyPos = m_BossCache->GetPos();
			const XMFLOAT3& enemyFoward = m_BossCache->GetForward();

			m_Target = enemyPos;
			m_Target.y += m_RendTargetPosY;

			m_Position.y = m_Target.y + m_RendPosY;
			m_Position.x = m_Target.x + (enemyFoward.x * m_RendLength);
			m_Position.z = m_Target.z + (enemyFoward.z * m_RendLength);

			m_Rotation = {};
			m_Rotation.y = m_PlayerCache->GetRot().y;
		}
		else if (m_StartEnemyDirection)
		{
			const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
			const XMFLOAT3& playerFoward = m_PlayerCache->GetForward();
			const XMFLOAT3& enemyPos = m_BossCache->GetPos();

			m_Target = playerPos;
			m_Target.y += OFFSET_TARGET_POS_Y;

			m_Position.y = m_Target.y;

			const XMVECTOR& p1 = XMLoadFloat3(&playerPos);
			const XMVECTOR& p2 = XMLoadFloat3(&m_Position);

			const XMVECTOR& diff = XMVectorSubtract(p2, p1);
			const XMVECTOR& lengthVec = XMVector3Length(diff);

			const float& length = XMVectorGetX(lengthVec);

			m_Length = length;

			// 最小値
			if (length <= LENGTH_DEFAULT)
			{
				m_Length = LENGTH_DEFAULT;
			}
			// 最大値
			if (length >= LENGTH_EXTRATTACK)
			{
				m_Length = LENGTH_EXTRATTACK;
			}

			m_Position.x = m_Target.x - (playerFoward.x * m_Length);
			m_Position.z = m_Target.z - (playerFoward.z * m_Length);

			m_Rotation = {};
			m_Rotation.y = m_PlayerCache->GetRot().y;
		}
		else
		{
			const XMFLOAT3& playerRight = m_PlayerCache->GetRight();
			m_Target = m_PlayerCache->GetPos();
			m_Target.y += CUTIN_TARGET_POS_Y;
			m_Position = m_Target;
			m_Position.x -= playerRight.x * LENGTH_CUTIN;
			m_Position.z -= playerRight.z * LENGTH_CUTIN;
		}
	}
}

void PlayerCamera::StartCutIn()
{
	m_CutInMode = true;
}

void PlayerCamera::EndCutIn()
{
	m_CutInMode = false;
	m_StartEnemyDirection = false;
}

void PlayerCamera::StartEnemyDirection()
{
	m_StartEnemyDirection = true;
}
