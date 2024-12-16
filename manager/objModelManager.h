#pragma once
#include <unordered_map>
#include <string>

// ëOï˚êÈåæ
struct MODEL;
struct MODEL_OBJ;
struct MODEL_MATERIAL;

enum class STATICMESH_MODEL
{
	SKYDOME = 0,
	BOX,
	BOX_CENTER,
	TREEGATE,
	COLLISION_BOX,
	MAX
};

class ObjModelManager final
{
private:
	static std::unordered_map<STATICMESH_MODEL, MODEL*> m_LoadModelPool;
	static std::unordered_map<STATICMESH_MODEL, std::string> m_ReservModelPool;

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);
	static void Load(const STATICMESH_MODEL& model,const std::string& fileName);
public:
	static void Init();
	static void Uninit();
	static void ReservModel(const STATICMESH_MODEL& model, const std::string& fileName);
	static const MODEL* GetModel(const STATICMESH_MODEL& model);
};