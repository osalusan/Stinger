#pragma once
#include "camera.h"

class Player;

class PlayerCamera final:public Camera
{
private:
	Player* m_PlayerCache = nullptr;

	XMFLOAT2 m_MousePos = {};
	XMFLOAT2 m_OldMousePos = {};
	int m_CurrentTime = 0;
public:
	using Camera::Camera;
	virtual ~PlayerCamera()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};