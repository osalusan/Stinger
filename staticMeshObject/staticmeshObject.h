#pragma once
#include "object/gameObject.h"

class ModelRenderer;

class StaticMeshObject :public GameObject
{
protected:
	ModelRenderer* m_ModelRenderer = nullptr;

	virtual void LoadStaticMesh() = 0;
	void LoadModel(const std::string& fileName);
public:
	virtual ~StaticMeshObject();
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override = 0;
	virtual void Draw()override;

	
};