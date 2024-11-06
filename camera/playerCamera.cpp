#include "playerCamera.h"
#include "character/player.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/gameScene.h"

PlayerCamera::~PlayerCamera()
{
	m_PlayerCashe = nullptr;
}

void PlayerCamera::Init()
{
	if (m_PlayerCashe == nullptr)
	{
		GameScene* scene = SceneManager::GetScene<GameScene>();
		if (scene == nullptr) return;
		ObjectManager* objectManager = scene->GetObjectManager();
		if (objectManager == nullptr) return;
		
		m_PlayerCashe = objectManager->GetPlayer();
	}
}

void PlayerCamera::Uninit()
{
	// 純粋仮想関数の為空実装
}

void PlayerCamera::Update()
{
	//POINT MousePos{};
	//GetCursorPos(&MousePos);

	//m_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));

	//m_length -= GetMouseWheelValue() * 0.01f;

	//if (m_length <= _LENGTH_MIN) { m_length = _LENGTH_MIN; }
	//else if (_length >= _LENGTH_MAX) { _length = _LENGTH_MAX; }
#if _DEBUG
	//if (Input::GetKeyPress('R'))
	//{
	//	SetCursorPos(GetWindowSize().x * 0.5f, GetWindowSize().y * 0.5f);
	//	m_OldMousePos = m_MousePos;
	//}

	//if (_MousePos.x <= 100.0f || _MousePos.x >= GetWindowSize().x - 100.0f || _MousePos.y <= 100.0f || _MousePos.y >= GetWindowSize().y - 100.0f)
	//{
	//	SetCursorPos(GetWindowSize().x * 0.5f, GetWindowSize().y * 0.5f);
	//	GetCursorPos(&MousePos);
	//	_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
	//	_OldMousePos = _MousePos;
	//}
#else
	if (_MousePos.x <= 100.0f || _MousePos.x >= GetWindowSize().x - 100.0f || _MousePos.y <= 100.0f || _MousePos.y >= GetWindowSize().y - 100.0f)
	{
		SetCursorPos(GetWindowSize().x * 0.5f, GetWindowSize().y * 0.5f);
		POINT MousePos{};
		GetCursorPos(&MousePos);
		_MousePos = XMFLOAT2(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y));
		_OldMousePos = _MousePos;
	}
#endif // _DEBUG

	////マウスの横移動処理
	//m_Rotation.y += (m_MousePos.x - m_OldMousePos.x) * 0.005f;
	////マウスの縦移動処理
	//m_Rotation.x -= (m_MousePos.y - m_OldMousePos.y) * 0.005f;
	////マウスの上下制限
	//if (m_Rotation.x > 1.14f)m_Rotation.x = 1.14f;
	//if (m_Rotation.x < -1.14f)m_Rotation.x = -1.14f;

	if (m_PlayerCashe != nullptr)
	{
		m_Target = m_PlayerCashe->GetPos();
	}

	//カメラの移動処理
	//m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_length;
	//m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_length;

	m_Position.y = m_Target.y + m_length;

	m_Position.z = m_Target.z - m_length;

	/*m_OldMousePos = m_MousePos;*/
}
