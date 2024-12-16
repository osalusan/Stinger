#pragma once
#include "object/gameObject.h"

// 前方宣言
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;

class ObjModelRenderer;
class StaticMeshObject;
class Character;
class MeshFiled;
class ShadowVolume final:public GameObject
{
protected:
	// モデル本体 / コンストラクタで初期化
	STATICMESH_MODEL m_StaticModel;						
	ANIMETION_MODEL m_AnimeModel;						

	ObjModelRenderer* m_ModelRenderer = nullptr;

	StaticMeshObject* m_StaticMeshCache = nullptr;
	Character* m_CharacterCache = nullptr;

	MeshFiled* m_MeshFiled = nullptr;
public:
	ShadowVolume() = delete;
	ShadowVolume(StaticMeshObject* staticMesh);
	ShadowVolume(Character* character);
	virtual ~ShadowVolume()override;
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};