#pragma once

#include "renderer/renderer.h"
#include <unordered_map>
#include <map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp/assimp-vc143-mt.lib")

// ボーンの最大数
#define BONES_MAX (256)

struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int				BoneNum= 0;
	int				BoneIndex[4] = {0};//本来はボーンインデックスで管理するべき
	float			BoneWeight[4] = {0.0f};
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix = { 0.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 0.0f };
	aiMatrix4x4 AnimationMatrix = { 0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f };
	aiMatrix4x4 OffsetMatrix = { 0.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 0.0f };
};

class FbxModelRenderer final
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation = {};

	ID3D11Buffer** m_VertexBuffer = nullptr;
	ID3D11Buffer** m_IndexBuffer = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture = {};

	// GPUスキニング用
	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone = {};						//ボーンデータ（名前で参照）
	std::unordered_map<int, std::string> m_NameList = {};


	XMFLOAT3 m_Center = {};
	XMFLOAT3 m_Scale = {};

public:
	void Load(const char* FileName);
	void Uninit();
	void Draw();

	void LoadAnimation(const char* FileName, const char* Name);
	void Update(const char* AnimationName1, const float& time1, const char* AnimationName2, const float& time2, float BlendRatio);
	void CreateBone(aiNode* node, std::map<std::string, int>& boneIndexMap, int& boneCount);
	void Update(const char* AnimationName1,const float& time);
	void UpdateBoneMatrix(aiNode* node,aiMatrix4x4 matrix);

	const XMFLOAT3& GetCenter()const
	{
		return m_Center;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
};