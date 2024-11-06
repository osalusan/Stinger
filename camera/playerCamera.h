#pragma once
#include "camera.h"

class Player;

class PlayerCamera :public Camera
{
private:
	Player* m_PlayerCashe = nullptr;

	XMFLOAT2 m_MousePos = {};
	XMFLOAT2 m_OldMousePos = {};
	int m_CurrentTime = 0;
public:
	using Camera::Camera;
	~PlayerCamera();
	void Init()override;
	void Uninit()override;
	void Update()override;
};