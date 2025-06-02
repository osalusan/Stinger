#pragma once
#include "scene/scene.h"
// タイトルのオブジェクトを管理するシーン
class TitleScene final :public Scene
{
private:
	bool m_NextScene = false;
	float m_NextSceneTime = 0.0f;
public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;

	void CreateShadowVolume();
};
