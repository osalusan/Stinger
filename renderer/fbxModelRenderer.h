#pragma once

#include "renderer/renderer.h"
#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp/assimp-vc143-mt.lib")

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position = { 0.0f,0.0f,0.0f };
	aiVector3D Normal = { 0.0f,0.0f,0.0f };
	int				BoneNum = 0;
	std::string		BoneName[4] = { "" };			//本来はボーンインデックスで管理するべき
	float			BoneWeight[4] = { 0 };
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

	std::vector<DEFORM_VERTEX>* m_DeformVertex = {};						//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone = {};						//ボーンデータ（名前で参照）

	XMFLOAT3 m_Center = {};
	XMFLOAT3 m_Scale = {};


public:
	void Load(const char* FileName);
	void Uninit();
	void Draw();

	void LoadAnimation(const char* FileName, const char* Name);
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void CreateBone(aiNode* node);
	void Update(const char* AnimationName1, float animationTime);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	aiQuaternion CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	aiVector3D CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const XMFLOAT3& GetCenter()const
	{
		return m_Center;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
};