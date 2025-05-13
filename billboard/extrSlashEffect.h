#pragma once
#include "billboard.h"

// 前方宣言
class Camera;
// エクストラスラッシュのビルボードエフェクト
class ExtrSlashEffect :public BillBoard
{
private:
	const GameObject* m_FollowObject = nullptr;
	const GameObject* m_HitEnemyCache = nullptr;
	const Camera* m_CameraCache = nullptr;

public:
	ExtrSlashEffect() = delete;
	ExtrSlashEffect(const GameObject* followObj);
	virtual void Update(const float& deltaTime)override;
	// 作成順番的に、後から格納しないといけない
	void SetEnemyPointer(const GameObject* hitEnemy);
};