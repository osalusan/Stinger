#pragma once
#include "character/character.h"
#include <unordered_map>

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;
class EquipmentObject;

class Player final:public Character {
private:

	BoxCollisionComponent* m_BoxCollCache = nullptr;
	BoxCollisionComponent* m_SwordBoxCollCache = nullptr;

	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	EquipmentObject* m_ShiledChache = nullptr;
	EquipmentObject* m_SwordChache = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_PlayerStateData = {};

	bool m_ChengeAnimation = false;				// アニメーションが今変えられるかどうか
	bool m_UseAttack = false;

	// データ保存用
	float m_RollingSpeedValue = 0.0f;
	float m_MinRollingTime = 0.0f;
	float m_MaxRollingTime = 0.0f;

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
	void SwordCollisionControl();
	void PlayerDataLoadCSV(const std::string& filePath);
public:
	Player() = delete;
	Player(const XMFLOAT3& pos);
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;

	virtual void TakeDamage(const float& atk)override;
	void TakeDamageParryPossible(const float& atk);
	void HitShiled(BoxCollisionComponent* boxColl);

	const PlayerStateMachine* GetPlayerStateMachine()
	{
		return m_PlayerStateMachine;
	}
	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
	}
	EquipmentObject* GetShiledCache()
	{
		return m_ShiledChache;
	}	
	EquipmentObject* GetSwordChache()const
	{
		return m_SwordChache;
	}

	void SetVelocityX(const float& x)
	{
		m_Velocity.x = x;
	}
	void SetVelocityZ(const float& z)
	{
		m_Velocity.z = z;
	}
	void SetVelocityY(const float& y)
	{
		m_Velocity.y = y;
	}
	void SetRotationY(const float& y)
	{
		m_Rotation.y = y;
	}

	const float& GetRollingSpeedValue()const
	{
		return m_RollingSpeedValue;
	}	
	const float& GetMinRollingTime()const
	{
		return m_MinRollingTime;
	}	
	const float& GetMaxRollingTime()const
	{
		return m_MaxRollingTime;
	}

	const std::unordered_map<std::string, float>& GetStateData(const std::string& skillName)const
	{
		if (m_PlayerStateData.count(skillName) >= 1)
		{
			return m_PlayerStateData.at(skillName);
		}
		// テンポラリ警告用
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};