#pragma once
#include "scene/scene.h"

class TitleScene final :public Scene
{
private:

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};
