#pragma once
#include "object/gameObject.h"

// 前方宣言
class ObjModelRenderer;
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;
struct MODEL;

class ShadowVolume final:public GameObject
{
protected:
	STATICMESH_MODEL m_StaticModel;						// モデル本体 / コンストラクタで初期化
	ANIMETION_MODEL m_AnimeModel;						// モデル本体 / コンストラクタで初期化
	ObjModelRenderer* m_ModelRenderer = nullptr;

public:
	ShadowVolume() = delete;
	ShadowVolume(const STATICMESH_MODEL& model);
	ShadowVolume(const ANIMETION_MODEL& model);
	virtual ~ShadowVolume()override;
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};