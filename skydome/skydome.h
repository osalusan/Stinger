#pragma once
#include "staticMeshObject/staticmeshObject.h"
#include <memory>

// ‘O•ûéŒ¾
class PlayerCamera;

class SkyDome final : public StaticMeshObject
{
private:
	std::shared_ptr<PlayerCamera> m_Camera = nullptr;

	virtual void LoadStaticMesh();
public:
	virtual ~SkyDome();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};