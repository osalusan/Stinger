#include "playerCamera.h"
#include "character/player.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "scene/gameScene.h"

constexpr float LENGTH = (1.0f / SCREEN_SCALE) * 70.0f;
constexpr float OFFSET_TARGET_POS_Y = LENGTH * 0.35f;
constexpr float LENGTH_CUTIN = (1.0f / SCREEN_SCALE) * 9.0f;
constexpr float CUTIN_TARGET_POS_Y = LENGTH_CUTIN * 1.25f;
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

	m_Length = LENGTH;
}

void PlayerCamera::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	if (!m_CutInMode)
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

		if (m_PlayerCache != nullptr)
		{
			m_Target = m_PlayerCache->GetPos();
			m_Target.y += OFFSET_TARGET_POS_Y;
		}

		//カメラの移動処理

		const float& cosPitch = cosf(m_Rotation.x);
		const float& sinPitch = sinf(m_Rotation.x);
		const float& sinYaw = sinf(m_Rotation.y);
		const float& cosYaw = cosf(m_Rotation.y);

		const float& horizontalDist = m_Length * cosPitch;

		m_Position.x = m_Target.x - horizontalDist * sinYaw;
		m_Position.z = m_Target.z - horizontalDist * cosYaw;
		m_Position.y = m_Target.y - m_Length * sinPitch;

		m_OldMousePos = m_MousePos;

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
	else
	{
		if (m_PlayerCache != nullptr)
		{
			m_Target = m_PlayerCache->GetPos();
			m_Target.y += CUTIN_TARGET_POS_Y;
			m_Position = m_PlayerCache->GetPos();
			m_Position.x -= m_PlayerCache->GetRight().x * LENGTH_CUTIN;
			m_Position.z -= m_PlayerCache->GetRight().z * LENGTH_CUTIN;
			m_Position.y += CUTIN_TARGET_POS_Y;
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
}
