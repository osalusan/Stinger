#pragma once
#include <string>
#include "component.h"
#include "renderer/renderer.h"

class ShaderComponent final:public Component
{
private:
	// •`‰æŠÖ˜A
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	bool m_IsUseRotMatrix = false;
	XMMATRIX m_RotScaleMatrix = {};
public:
	using Component::Component;
	ShaderComponent(GameObject* gameObject,const std::string& vsFileName, const std::string& psFileName);
	virtual ~ShaderComponent()override;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;

	void SetRotMatrix(const XMMATRIX& rotMatrix);
};