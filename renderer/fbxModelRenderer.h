#pragma once
#include "renderer/renderer.h"
#include <unordered_map>
#include <map>
#include "externalLibrary/assimp/cimport.h"
#include "externalLibrary/assimp/scene.h"
#include "externalLibrary/assimp/postprocess.h"
#include "externalLibrary/assimp/matrix4x4.h"
#include "externalLibrary/assimp/Importer.hpp"
#pragma comment (lib, "externalLibrary/assimp/assimp-vc143-mt.lib")

// シェーダー側と同じになるように
#define BONE_MAX (256)

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix = { 1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f };
	aiMatrix4x4 AnimationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f };
	aiMatrix4x4 OffsetMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f };

	aiMatrix4x4 LocalMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f };
};
// FBXの読み込み処理を行うクラス
class FbxModelRenderer final
{
private:
	const aiScene* m_AiScene = nullptr;
	Assimp::Importer m_Importer;
	std::unordered_map<std::string, const aiScene*> m_Animation = {};

	ID3D11Buffer** m_VertexBuffer = nullptr;
	ID3D11Buffer** m_IndexBuffer = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture = {};

	float m_MaxAnimeTime = 0.0f;

	// 格納用
	std::unordered_map<std::string, BONE> m_Bone = {};						//ボーンデータ（名前で参照）
	// GPUスキニング用	
	std::vector<BONE*> m_BoneIndex;											// ボーンインデックス順のボーンリスト


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
	
	const std::unordered_map<std::string, BONE>& GetBone()const
	{
		return m_Bone;
	}
	const XMFLOAT3& GetCenter()const
	{
		return m_Center;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
	float GetMaxAnimeTime(const std::string& name)const;

	// assimp用のマトリクスからDX用のマトリクスへ
	XMMATRIX AiMatrixToXMMATRIX(const aiMatrix4x4& mat) const
	{
		return XMMATRIX(
			mat.a1, mat.b1, mat.c1, mat.d1,
			mat.a2, mat.b2, mat.c2, mat.d2,
			mat.a3, mat.b3, mat.c3, mat.d3,
			mat.a4, mat.b4, mat.c4, mat.d4
		);
	}
};