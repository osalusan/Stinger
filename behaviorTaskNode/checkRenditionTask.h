#pragma once
#include "behaviorNode/taskNode.h"

// 前方宣言
class Camera;
// ---------------------------------------------------------
// タスクの説明：敵の演出を管理するタスク
// 備考：子クラスに演出を行いたい順番で追加
// ---------------------------------------------------------
class CheckRenditionTask final : public TaskNode
{
private:
	int m_TaskCount = 0;
	bool m_AcceptRendition = false;
	Camera* m_CameraCache = nullptr;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
