#pragma once
#include <unordered_map>
#include <string>

// 前方宣言
struct MODEL;
struct MODEL_OBJ;
struct MODEL_MATERIAL;
class ObjModelRenderer;
// 読み込んでいるobjモデルの一覧
enum class STATICMESH_MODEL
{
	SKYDOME_TITLE = 0,
	SKYDOME_BATTLE_MAWJ,
	BOX,
	BOX_CENTER,
	TREEGATE,
	COLLISION_BOX,
	RUINS,
	SOWRD,
	SHIELD,
	SPHERE,
	MAX
};
// ゲーム内すべてのobjモデルを管理するクラス
class ObjModelManager final
{
private:
	static std::unordered_map<STATICMESH_MODEL, ObjModelRenderer*> m_LoadModelPool;
	static std::unordered_map<STATICMESH_MODEL, std::string> m_ReservModelPool;

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);
	static void Load(const STATICMESH_MODEL& model,const std::string& fileName);
public:
	static void Init();				// 予約されたモデルをSceneの最後に一括で読み込む
	static void Uninit();
	static void ReservModel(const STATICMESH_MODEL& model, const std::string& fileName);
	static ObjModelRenderer* GetModel(const STATICMESH_MODEL& model);
};