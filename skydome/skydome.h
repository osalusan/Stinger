#pragma once
#include "staticMeshObject/staticMeshObject.h"
#include <memory>

// 前方宣言
class Camera;
// 空の描画をするクラス
class SkyDome final : public StaticMeshObject
{
private:
	Camera* m_CameraCache = nullptr;	// ドームがカメラについて行くように

	virtual void MoveControl(const float& deltaTime)override;
public:
	SkyDome() = delete;
	SkyDome(const STATICMESH_MODEL& model,std::string filePath);
	virtual ~SkyDome();
	virtual void Init()override;
};