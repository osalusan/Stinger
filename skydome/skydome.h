#pragma once
#include "staticMeshObject/staticmeshObject.h"
#include <memory>

// ‘O•ûéŒ¾
class Camera;

class SkyDome final : public StaticMeshObject
{
private:
	Camera* m_CameraCache = nullptr;

public:
	SkyDome();
	virtual ~SkyDome();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
};