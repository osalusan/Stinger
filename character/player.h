#pragma once
#include "character/character.h"
#include <unordered_map>

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;
class EquipmentObject;
class Polygon2D;
// プレイヤーの当たり判定の生成やモデルを管理しているクラス
class Player final:public Character {
private:

	BoxCollisionComponent* m_BoxCollCache = nullptr;			// プレイヤー自身の当たり判定
	Polygon2D* m_PlayerHpCache = nullptr;						// プレイヤーの体力表記 / HPが変化した時のみに反映させる為、プレイヤーで管理
	PlayerStateMachine* m_PlayerStateMachine = nullptr;			// プレイヤーステートマシーン

	// プレイヤーの装備
	EquipmentObject* m_SwordChache = nullptr;					// 攻撃の処理をプレイヤーのパラメータで行うため、プレイヤーで管理
	EquipmentObject* m_ShiledChache = nullptr;					// パリィの処理をプレイヤーで行う為、プレイヤーで管理
	BoxCollisionComponent* m_SwordBoxCollCache = nullptr;		// 剣の当たり判定のみ / 判定を取るたび剣のオブジェクトからGetComponentしなくていいように

	// CSVから読み込んだステータスをここに保存
	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_PlayerStateData = {};

	bool m_ChengeAnimation = false;				// アニメーションが今変えられるかどうか			
	float m_CurTakeNoDmgTime = 0.0f;			// ダメージを喰らわないでいる時間
	float m_HealStartTime = 0.0f;				// ダメージを喰らわないでいた時の、回復開始時間 
	float m_HealSpeed = 0.0f;					// 回復速度

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;

	void SwordCollisionControl();								// 剣の当たり判定制御と敵にダメージを与える処理
	void PlayerDataLoadCSV(const std::string& filePath);		// CSVからプレイヤーパラメータの取得
public:
	Player() = delete;
	Player(const XMFLOAT3& pos);
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;
	// 回避中はダメージを受けないように / エネミーがパリィ不可の攻撃を使用する場合は、この処理を呼ぶ
	virtual void TakeDamage(const float& atk)override;
	// パリィ中はダメージを受けないように / エネミーがパリィ可能な攻撃を使用する場合は、この処理を呼ぶ
	void TakeDamageParryPossible(const float& atk);
	// エネミーの方で盾に当たっていたらダメージ処理ではなく、この処理を呼ぶように
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
	// 各プレイヤーステートで、技の名前を指定してステータスを取得する
	const std::unordered_map<std::string, float>& GetStatusData(const std::string& skillName)const
	{
		if (m_PlayerStateData.count(skillName) >= 1)
		{
			return m_PlayerStateData.at(skillName);
		}
		// テンポラリ警告を削除する為に作成
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};