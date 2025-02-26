#include "playerStateExtrAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
#include "manager/audioManager.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"
#include "camera/playerCamera.h"

constexpr float SLOW_ANIMATION_SPEED = 0.1f;
constexpr float QUICK_ANIMATION_SPEED = 1.3f;

void PlayerStateExtrAttack::Init()
{
	m_CurrentTime = 0.0f;
	m_AttackAccept = false;
	m_StopAnim = false;
	m_CurrentStopAnimTime = 0.0f;
	m_CurrentMoveTime = 0.0f;
	m_AnimSpeedValue = 1.0f;
	m_MoveSpeedVec = {};
	m_AttackEnable = false;
	m_ChageSound = false;

	if (m_PlayerMachine == nullptr) return;

	m_PlayerMachine->UseExtrAttack();

	if (m_PlayerCache == nullptr)
	{
		// Getのみ / 編集不可
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (!m_Load && m_PlayerCache != nullptr)
	{
		LoadAnimation("asset\\model\\player\\swordAndShieldExtrAttack_PaladinJNordstrom.fbx", "extrAttack");

		AudioManager::ReservAudio(AUDIO::SOWRD_CHAGE, "asset\\audio\\se\\sowrdChage.wav");
		AudioManager::ReservAudio(AUDIO::SLASH3_SE, "asset\\audio\\se\\slash3.wav");

		const std::unordered_map<std::string, float>& parryAttak = m_PlayerCache->GetStateData("エクストラ攻撃");

		m_DamageValue = FindStateData(parryAttak, "ダメージ_倍率");
		m_CutInTimeMin = FindStateData(parryAttak, "カットイン時間_最小");
		m_CutInTimeMax = FindStateData(parryAttak, "カットイン時間_最大");
		m_MoveTimeMax = FindStateData(parryAttak, "移動時間_最大");
		m_StartStopAnimTimeValue = FindStateData(parryAttak, "アニメーション速度制御開始時間_割合");
		m_StopAnimTimeValue = FindStateData(parryAttak, "アニメーション速度制御時間_割合");

		m_Load = true;
	}

	if (m_ObjManagerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		m_ObjManagerCache = scene->GetObjectManager();
	}

	if (m_BossCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_BossCache = m_ObjManagerCache->GetBossEnemy();
		}
	}

	if (m_CameraCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_CameraCache = dynamic_cast<PlayerCamera*>(m_ObjManagerCache->GetCamera());
		}
	}	

	if (m_BossCache != nullptr)
	{
		const XMFLOAT3& myPos = m_PlayerCache->GetPos();
		const XMFLOAT3& targetPos = m_BossCache->GetPos();

		m_MoveSpeedVec.x = (targetPos.x - myPos.x) / m_MoveTimeMax;
		m_MoveSpeedVec.z = (targetPos.z - myPos.z) / m_MoveTimeMax;

		float currentAngle = m_PlayerCache->GetRot().y;

		float targetAngle = atan2f(targetPos.x - myPos.x, targetPos.z - myPos.z);

		float angleDiff = targetAngle - currentAngle;
		while (angleDiff > XM_PI)
		{
			angleDiff -= XM_2PI;
		}
		while (angleDiff < -XM_PI)
		{
			angleDiff += XM_2PI;
		}

		m_PlayerMachine->SetRotationY(currentAngle + angleDiff);
	}

	if (m_ObjManagerCache != nullptr && m_PlayerMachine->GetIsExtrAttack())
	{
		// TODO :エネミーの動きのみを止める処理

		m_ObjManagerCache->SetSlowTimeEnemy(m_CutInTimeMax + m_MoveTimeMax);
		m_ObjManagerCache->SetSlowValue(0.0f);
	}
}

void PlayerStateExtrAttack::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateExtrAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime * m_AnimSpeedValue;

	if (m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimName);
		}
	}

	// カットイン制御
	if (m_CurrentTime >= m_MaxAnimTime * m_CutInTimeMin)
	{
		if (m_CameraCache != nullptr)
		{
			m_CameraCache->StartCutIn();

			if (!m_ChageSound && m_CurrentTime >= (m_MaxAnimTime * m_CutInTimeMax) * 0.5f)
			{
				m_ChageSound = true;
				AudioManager::Play(AUDIO::SOWRD_CHAGE, false, 0.75f);
			}
		}
	}

	if (m_PlayerMachine == nullptr) return;

	// アニメーションの停止
	if (m_CurrentStopAnimTime == 0.0f)
	{
		if (m_CurrentTime >= m_MaxAnimTime * m_StartStopAnimTimeValue)
		{
			m_AnimSpeedValue = SLOW_ANIMATION_SPEED;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			m_StopAnim = true;
		}
	}

	// カットイン終了後
	if (m_StopAnim)
	{
		m_CurrentStopAnimTime += deltaTime;

		if (m_CurrentStopAnimTime >= m_StopAnimTimeValue)
		{
			m_AnimSpeedValue = 0.0f;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			m_StopAnim = false;
			m_CameraCache->StartEnemyDirection();
		}
	}
	// エネミーの手前まで移動
	else
	{
		if (m_CurrentStopAnimTime >= m_StopAnimTimeValue)
		{
			m_CurrentMoveTime += deltaTime;
		}

		// 移動
		if (m_CurrentMoveTime != 0.0f && m_CurrentMoveTime <= m_MoveTimeMax)
		{
			m_PlayerMachine->SetVelocityX(m_MoveSpeedVec.x);
			m_PlayerMachine->SetVelocityZ(m_MoveSpeedVec.z);
		}
		else if (m_CurrentMoveTime >= m_MoveTimeMax)
		{
			m_AttackEnable = true;
			m_PlayerMachine->InitVelocity();
			m_AnimSpeedValue = QUICK_ANIMATION_SPEED;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
		}
	}
}

void PlayerStateExtrAttack::ChangeStateControl()
{
	if (m_CurrentTime >= m_MaxAnimTime)
	{		
		if (m_PlayerMachine != nullptr)
		{
			m_AnimSpeedValue = 1.0f;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
		}

		if (m_CameraCache != nullptr)
		{
			m_CameraCache->EndCutIn();
		}

		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

bool PlayerStateExtrAttack::CheckAttackAccept()
{
	// カットイン終了後に判定を出す
	if (!m_AttackAccept && m_CurrentTime >= m_MaxAnimTime * m_CutInTimeMin)
	{
		if (m_BossCache == nullptr || m_PlayerCache == nullptr)
		{
			return false;
		}

		m_BossCache->TakeDamage(m_PlayerCache->GetAttack() * m_DamageValue);
		m_BossCache->SetParryRecoil(true);
		
		m_AttackAccept = true;

		AudioManager::Play(AUDIO::SLASH3_SE, false, 0.85f);
	}
	return m_AttackAccept;
}
