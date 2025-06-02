#pragma once
#include "camera.h"
// ロード画面で使用するカメラ
class LoadCamera final :public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};