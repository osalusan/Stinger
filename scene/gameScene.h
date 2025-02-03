#pragma once
#include "scene/scene.h"

class GameScene final :public Scene 
{

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};