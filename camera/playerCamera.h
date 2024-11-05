#pragma once
#include "camera.h"
#include <memory>

class Player;

class PlayerCamera :public Camera
{
private:
	std::shared_ptr<Player> m_Player = nullptr;

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