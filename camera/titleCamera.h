#pragma once
#include "camera.h"
// タイトル画面で使用するカメラ
class TitleCamera final:public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};