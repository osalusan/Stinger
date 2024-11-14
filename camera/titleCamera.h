#pragma once
#include "camera.h"

class TitleCamera :public Camera
{
public:
	using Camera::Camera;
	void Init()override;
	void Uninit()override;
	void Update()override;
};