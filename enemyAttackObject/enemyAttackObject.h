#pragma once
#include "object/gameObject.h"

class BoxCollisionComponent;
// 敵から放たれる攻撃の基底クラス
class EnemyAttackObject : public GameObject
{
protected:
	const GameObject* m_TargetObject = nullptr;
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	bool m_IsAttack = false;
	float m_Damage = 0.0f;
	virtual void AttackControl(const float& deltaTime) = 0;			// 攻撃が有効になっている時の処理をここで行う
	virtual bool CollisionDamageControl();							// プレイヤーに当たった時のダメージ処理をここで行う
	virtual void Finish() = 0;										// 攻撃が終了した際に、ここで数値をリセットする
public:
	EnemyAttackObject() = delete;
	EnemyAttackObject(const GameObject* target);
	virtual void Update(const float& deltaTime)override final;

	// タスクから攻撃を生成する時に使用
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& lifeTime);		// 座標とダメージと制限時間を設定して生成

	// 生成してある攻撃に変更を加えるときに使用
	virtual void Attack() = 0;							// 生成と攻撃処理を分けて行いたい時用に
	virtual void KeepPos(const XMFLOAT3& shotPos);		// 座標を固定
};