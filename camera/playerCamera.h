#pragma once
#include "camera.h"

class Player;
class BossEnemy;
// プレイヤーに追従するカメラ
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
	bool m_CheckDistance = false;
	int m_CurrentTime = 0;
	XMFLOAT2 m_MouseSpeed = {};
	bool m_PushMouse = false;
public:
	using Camera::Camera;
	virtual ~PlayerCamera()override;
	void Init()override;
	void Update(const float& deltaTime)override;

	void StartCutIn();
	void EndCutIn();
	void StartEnemyDirection();
};