#pragma once

#include "renderer/renderer.h"
#include <unordered_map>
#include <map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp/assimp-vc143-mt.lib")

// �{�[���̍ő吔
#define BONES_MAX (256)

struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int				BoneNum= 0;
	int				BoneIndex[4] = {0};
	float			BoneWeight[4] = {0.0f};
};

//�{�[���\����
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
};

class FbxModelRenderer final
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation = {};

	ID3D11Buffer** m_VertexBuffer = nullptr;
	ID3D11Buffer** m_IndexBuffer = nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture = {};
	std::vector<DEFORM_VERTEX>* m_DeformVertex;//�ό`�㒸�_�f�[�^

	// GPU�X�L�j���O�p
	std::unordered_map<std::string, BONE> m_Bone = {};						//�{�[���f�[�^�i���O�ŎQ�Ɓj
	std::vector<BONE*> m_BonesByIndex;										// �{�[���C���f�b�N�X���̃{�[�����X�g
	std::unordered_map<int, std::string> m_NameList = {};


	XMFLOAT3 m_Center = {};
	XMFLOAT3 m_Scale = {};


	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);
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