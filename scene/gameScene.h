#pragma once
#include "scene/scene.h"

// ‘O•ûéŒ¾
class PlayerCamera;

class GameScene final :public Scene 
{

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};