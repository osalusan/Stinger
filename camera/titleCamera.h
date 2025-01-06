#pragma once
#include "camera.h"

class TitleCamera final:public Camera
{
public:
	using Camera::Camera;
	virtual void Init()override;
};