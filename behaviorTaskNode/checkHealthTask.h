#pragma once
#include "behaviorNode/taskNode.h"
// ‘Ì—Í‚ª‚Ç‚ê‚­‚ç‚¢‚©”»’f‚µA¶‚«‚Ä‚¢‚é‚©€‚ñ‚Å‚¢‚é‚©‚ğ”»•Ê

class CheckHealthTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};