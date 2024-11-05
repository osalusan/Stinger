#pragma once
#include "character/character.h"

enum class PLAYER_STATE {
	IDEL = 0,
	WALK,
	ATTACK,
	DOWN,
	MAX
};

class Player :public Character {
private:

	bool _chengeAnimation = false;// アニメーションが今変えられるかどうか
	int _animationFrame = 0;	  // アニメーションのフレーム
	std::string _animationName = "Idle";// アニメーションの名前
	std::string _nextanimationName = "Idle";// ブレンド用、次のアニメーションの名前
	float _blendRatio = 1.0f;// アニメーションブレンドの数値
	bool _useAttack = false;
	
	virtual void MoveControl(const float& deltaTime)override;
	virtual void CollisionControl();
public:
	Player();
	virtual void Init()override;
	virtual void Uninit()override;
};