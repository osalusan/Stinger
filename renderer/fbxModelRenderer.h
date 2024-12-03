#pragma once

#include "renderer/renderer.h"
#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp/assimp-vc143-mt.lib")

#define MAX_BONE (256)
struct ANIMATIONBUFF
{
	int maxVertexCount = 0;    // モデルの最大頂点数
	XMFLOAT4X4 boneMatrix[MAX_BONE] = {}; // 各ボーンの変換行列
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

	unsigned int totalVertexCount = 0;
	std::vector<ID3D11Buffer*> notDeformVertexBuffers;
	std::vector<ID3D11ShaderResourceView*> notDeformVertexSRVs;
	std::vector<ID3D11Buffer*> vertexBuffers;
	std::vector<ID3D11ShaderResourceView*> updatedSRVs;
	std::vector<ID3D11UnorderedAccessView*> vertexUAVs;
	std::vector<ID3D11Buffer*> constantBuffers;
	std::vector<ID3D11Buffer*> indexBuffers;

	ID3D11ComputeShader* _skinCS = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture = {};

	std::unordered_map<std::string, BONE> m_Bone = {};						//ボーンデータ（名前で参照）

	XMFLOAT3 m_Center = {};
	XMFLOAT3 m_Scale = {};


public:
	void Load(const char* FileName);
	void Uninit();
	void Draw();

	void LoadAnimation(const char* FileName, const char* Name);
	void Update(const char* AnimationName1, const float& time1, const char* AnimationName2, const float& time2, float BlendRatio);
	void CreateBone(aiNode* node);
	void Update(const char* AnimationName1,const float& time);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	const XMFLOAT3& GetCenter()const
	{
		return m_Center;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
};