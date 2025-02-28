#pragma once
#include "billboard.h"

// ‘O•ûéŒ¾
class Camera;

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
	// ì¬‡”Ô“I‚ÉAŒã‚©‚çŠi”[‚µ‚È‚¢‚Æ‚¢‚¯‚È‚¢
	void SetEnemyPointer(const GameObject* hitEnemy);
};