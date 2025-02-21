#pragma once
#include "camera.h"

class Player;
class BossEnemy;

class PlayerCamera final:public Camera
{
private:
	Player* m_PlayerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;

	XMFLOAT2 m_MousePos = {};
	XMFLOAT2 m_OldMousePos = {};
	XMFLOAT3 m_CutInRecordPos = {};
	XMFLOAT3 m_CutInPos = {};

	bool m_CutInMode = false;
	bool m_StartEnemyDirection = false;
	int m_CurrentTime = 0;
public:
	using Camera::Camera;
	virtual ~PlayerCamera()override;
	void Init()override;
	void Update(const float& deltaTime)override;

	void StartCutIn();
	void EndCutIn();
	void StartEnemyDirection();
};