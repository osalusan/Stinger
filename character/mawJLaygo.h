#pragma once
#include "bossEnemy.h"
#include <unordered_map>

enum class MAWJ_ANIMATION
{
	NONE = 0,
	ROARING
};

// 前方宣言
struct BONE;
// MawJLaygoの当たり判定の生成やモデルを管理しているクラス
class MawJLaygo final:public BossEnemy
{
private:
	// 使用アニメーション管理用
	std::unordered_map<MAWJ_ANIMATION, std::string> m_MawJAnimationName = {};

	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
public:
	using BossEnemy::BossEnemy;

	virtual ~MawJLaygo()override;
	virtual void Init()override;


	const std::string GetMawJAnimationName(const MAWJ_ANIMATION& name)
	{
		return m_MawJAnimationName[name];
	}
};