#pragma once
#include "staticMeshObject/staticmeshObject.h"
#include <memory>

// ‘O•ûéŒ¾
class PlayerCamera;

class SkyDome final : public StaticMeshObject
{
private:
	PlayerCamera* m_CameraCashe = nullptr;

public:
	SkyDome();
	virtual ~SkyDome();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};