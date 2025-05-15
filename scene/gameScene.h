#pragma once
#include "scene/scene.h"
// ゲーム中のオブジェクトを管理するシーン
class GameScene final :public Scene 
{
private:
	float m_CurrentClearTime = 0.0f;
	float m_CurrentGameOverTime = 0.0f;
	int m_SelectKey = 0;
public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};