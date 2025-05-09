#pragma once
#include "behaviorNode/taskNode.h"

class Camera;
// ---------------------------------------------------------
// タスクの説明：読み込み時に使う、待機用アニメーション
// 備考：
// ---------------------------------------------------------
class LoadIdleTask final : public TaskNode
{
private:
	Camera* m_CameraCache = nullptr;

public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};