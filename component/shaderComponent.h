#pragma once
#include <string>
#include "component.h"
#include "renderer/renderer.h"
// 描画用のコンポーネント
class ShaderComponent final:public Component
{
private:
	// 描画関連
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	bool m_IsUseRotMatrix = false;				// GameObjectのrotationとは別の、回転の数値を反映させたい場合に描画の方も反映させる
	XMMATRIX m_RotScaleMatrix = {};				// 回転とスケール情報を保存
public:
	using Component::Component;
	ShaderComponent(GameObject* gameObject,const std::string& vsFileName, const std::string& psFileName);
	virtual ~ShaderComponent()override;
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;

	void SetRotMatrix(const XMMATRIX& rotMatrix);		// 回転がroll,pitch.yow通りじゃなければ、計算後の数値を格納
};