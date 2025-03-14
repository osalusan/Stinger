#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

// ‘O•ûéŒ¾
class Polygon2D;

class BattleStartRenditionTask final : public TaskNode
{
private:
	Polygon2D* m_BattleStartTextCache = nullptr;
	XMFLOAT2 m_BSTextUV = { 0.0f,1.0f };
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};