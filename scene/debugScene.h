#pragma once
#include "scene/scene.h"
// デバッグで使用したい時のシーン
class DebugScene final :public Scene
{

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};