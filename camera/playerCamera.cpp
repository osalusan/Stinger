#include "playerCamera.h"
#include "character/player.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "scene/gameScene.h"

constexpr float LENGTH = 30.0f;
// マウスの設定
constexpr XMINT2 CENTER = { SCREEN_WIDTH / 2  , SCREEN_HEIGHT / 2 };
constexpr XMFLOAT2 MOUSE_SPEED = { 0.005f,0.005f };
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

	POINT MousePos{};
	GetCursorPos(&MousePos);

	m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));

	//マウスの横移動処理
	m_Rotation.y += (m_MousePos.x - m_OldMousePos.x) * MOUSE_SPEED.x;
	//マウスの縦移動処理
	m_Rotation.x -= (m_MousePos.y - m_OldMousePos.y) * MOUSE_SPEED.y;
	//マウスの上下制限
	if (m_Rotation.x > 1.14f)
	{
		m_Rotation.x = 1.14f;
	}
	if (m_Rotation.x < -1.14f)
	{
		m_Rotation.x = -1.14f;
	}

	if (m_PlayerCache != nullptr)
	{
		m_Target = m_PlayerCache->GetPos();
	}

	//カメラの移動処理
	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_Length;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_Length;

	m_Position.y = m_Target.y - sinf(m_Rotation.x) * m_Length;

	m_OldMousePos = m_MousePos;

#if _DEBUG
	if (InputManager::GetKeyPress('R'))
	{
			SetCursorPos(CENTER.x, CENTER.y);
			GetCursorPos(&MousePos);
			m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
			m_OldMousePos = m_MousePos;
	}

	//if (m_MousePos.x <= MOUSE_MOVE_SPACE || m_MousePos.x >= CENTER.x - MOUSE_MOVE_SPACE || 
	//	m_MousePos.y <= MOUSE_MOVE_SPACE || m_MousePos.y >= CENTER.y - MOUSE_MOVE_SPACE)
	//{
	//	SetCursorPos(CENTER.x, CENTER.y);
	//	GetCursorPos(&MousePos);
	//	m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
	//	m_OldMousePos = m_MousePos;
	//}
#else
	// TODO:デバッグ用 / 変更予定
	if (InputManager::GetKeyPress('R'))
	{
		SetCursorPos(CENTER.x, CENTER.y);
		GetCursorPos(&MousePos);
		m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
		m_OldMousePos = m_MousePos;
	}
	//if (m_MousePos.x <= MOUSE_MOVE_SPACE || m_MousePos.x >= CENTER.x - MOUSE_MOVE_SPACE || 
	//	m_MousePos.y <= MOUSE_MOVE_SPACE || m_MousePos.y >= CENTER.y - MOUSE_MOVE_SPACE)
	//{
	//	SetCursorPos(CENTER.x, CENTER.y);
	//	GetCursorPos(&MousePos);
	//	m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
	//	m_OldMousePos = m_MousePos;
	//}
#endif // _DEBUG


}
