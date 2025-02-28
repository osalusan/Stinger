#include "extrSlashEffect.h"
#include "manager/textureManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "camera/camera.h"

constexpr float HIGHT_OFFSET_VALUE = 0.2f;
constexpr float SCALE_OFFSET_VALUE = 2.0f;

ExtrSlashEffect::ExtrSlashEffect(const GameObject* followObj)
	:BillBoard(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), TEXTURE::EFFECT_SLASH, L"asset\\texture\\sprite\\swordEffect.png", 0.054f, XMINT2(2, 6))
{
	m_FollowObject = followObj;
	m_Enable = false;
	m_Loop = false;
	m_BillboardY = true;
}

void ExtrSlashEffect::Update(const float& deltaTime)
{
	BillBoard::Update(deltaTime);
	if (m_HitEnemyCache == nullptr) return;

	m_Position = m_HitEnemyCache->GetPos();

	// インスタンス化の順番的に、コンストラクタに配置しても取得できない
	if (m_CameraCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;
		m_CameraCache = objManager->GetCamera();
	}

	if (m_CameraCache != nullptr)
	{
		const float& length = m_CameraCache->GetLength();
		m_Scale.x = length * SCALE_OFFSET_VALUE;
		m_Scale.y = length * SCALE_OFFSET_VALUE;
		m_Position.y = m_CameraCache->GetPos().y;
	}
}

void ExtrSlashEffect::SetEnemyPointer(const GameObject* hitEnemy)
{
	m_HitEnemyCache = hitEnemy;
}
