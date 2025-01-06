#pragma once
#include "camera.h"

class LoadCamera final :public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
};