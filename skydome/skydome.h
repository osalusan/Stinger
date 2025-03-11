#pragma once
#include "staticMeshObject/staticmeshObject.h"
#include <memory>

// ‘O•ûéŒ¾
class Camera;

class SkyDome final : public StaticMeshObject
{
private:
	Camera* m_CameraCache = nullptr;

	virtual void MoveControl(const float& deltaTime)override;
public:
	SkyDome() = delete;
	SkyDome(std::string filePath);
	virtual ~SkyDome();
	virtual void Init()override;
};