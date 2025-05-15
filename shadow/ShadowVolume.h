#pragma once
#include "object/gameObject.h"

// 前方宣言
enum class STATICMESH_MODEL;
enum class ANIMETION_MODEL;

class StaticMeshObject;
class Character;
class MeshFiled;
// ステンシルシャドウを使うクラス
class ShadowVolume final:public GameObject
{
protected:
	// モデル本体 / コンストラクタで初期化
	STATICMESH_MODEL m_StaticModel;						
	ANIMETION_MODEL m_AnimeModel;						

	StaticMeshObject* m_StaticMeshCache = nullptr;		// 影として使用
	Character* m_CharacterCache = nullptr;				// モデルのアニメーションを反映させるように / ステンシルシャドウだと上手く影を表現できないので、違う影を実装した方がいい

	MeshFiled* m_MeshFiled = nullptr;					// 影を地面の高さに描画
public:
	ShadowVolume() = delete;
	ShadowVolume(StaticMeshObject* staticMesh);			// 影として描画したいStaticMeshを引数に
	ShadowVolume(Character* character);					// 影として描画したいCharacterを引数に
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
};