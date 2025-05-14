#pragma once
#include "object/gameObject.h"

// 前方宣言
class BoxCollisionComponent;
// 当たり判定のついている透明な壁
class TransparentWall :public GameObject
{
private:
	BoxCollisionComponent* m_BoxCollCache = nullptr;		// 透明な壁 / デバッグ画面では表示される
 public:
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale);
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale,const XMFLOAT3& rot);

	 virtual void Init()override;
};