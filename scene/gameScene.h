#pragma once
#include "scene/scene.h"

// �O���錾
class PlayerCamera;

class GameScene final :public Scene 
{

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};