#pragma once
#include "staticMeshObject.h"
// 不規則に動く正方形のボックス / タイトル用
class MoveRandSizeBox final: public StaticMeshObject
{
private:
	float m_Speed = 0.0f;				// 移動速度
	float m_ResetPosZ = 0.0f;			// 一定以上のZ座標でリセット 
	float m_UpDownRange = 0.0f;			// 上下の幅
	float m_DefaultPosZ = 0.0f;			// 飛んでくる場所
	float m_RandCenterPosY = 0.0f;		// 基本となる高さ

	XMFLOAT3 m_RotValue = {};

	bool m_Up = false;

	virtual void MoveControl(const float& deltaTime)override;
public:
	MoveRandSizeBox();
	MoveRandSizeBox(const float& resetPosZ, const float& posZ);
};