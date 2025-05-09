#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

// 前方宣言
class Polygon2D;
// ---------------------------------------------------------
// タスクの説明：バトルスタート演出
// 備考：キャラクターのアニメーションやテクスチャ演出を実行
// ---------------------------------------------------------
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