#include "main/main.h"
#include "renderer/fbxModelRenderer.h"

void FbxModelRenderer::Draw()
{
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];


		// �}�e���A���ݒ�
		aiString texture;
		aiColor3D diffuse;
		float opacity;

		aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);

		if (texture == aiString(""))
		{
			material.TextureEnable = false;
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
			material.TextureEnable = true;
		}

		material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void FbxModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void FbxModelRenderer::CreateBone(aiNode* node, std::map<std::string, UINT>& boneIndexMap, int& boneCount)
{
	// �{�[�������擾
	std::string boneName = node->mName.C_Str();

	// �{�[�����܂��}�b�v�ɑ��݂��Ȃ��ꍇ�A�C���f�b�N�X�����蓖�Ă�
	if (boneIndexMap.find(boneName) == boneIndexMap.end())
	{
		boneIndexMap[boneName] = boneCount++;
	}

	// BONE �\���̂��쐬�܂��͎擾
	BONE& bone = m_Bone[boneName];

	// �q�m�[�h���ċA�I�ɏ���
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		CreateBone(node->mChildren[i], boneIndexMap, boneCount);
	}
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time)
{
	if (m_Animation.count(AnimationName1) == 0)return;
	if (!m_Animation[AnimationName1]->HasAnimations())return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	float TicksPerSecond;
	if (animation1->mTicksPerSecond != 0.0f)
	{
		TicksPerSecond = static_cast<float>(animation1->mTicksPerSecond);
	}
	else
	{
		TicksPerSecond = 60.0f; // �f�t�H���g�̃e�B�b�N���[�g
	}
	float TimeInTicks = time * TicksPerSecond;

	// �A�j���[�V�������Ԃ����[�v������
	float AnimationTime = static_cast<float>(fmod(TimeInTicks, animation1->mDuration));

	for (std::pair<const std::string, BONE>& pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1)
		{
			// �L�[�t���[���̑���
			unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
			unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

			// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
			f = 0;
			for (unsigned int i = 0; i < numRotKeys; i++)
			{
				if (AnimationTime < nodeAnim1->mRotationKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = 0;
			for (unsigned int i = 0; i < numPosKeys; i++)
			{
				if (AnimationTime < nodeAnim1->mPositionKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}
		else
		{
			rot1 = aiQuaternion();
			pos1 = aiVector3D(0.0f, 0.0f, 0.0f); 
		}

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	std::vector<XMFLOAT4X4> boneMatrices;
	for (const auto& bone : m_Bone) 
	{
		aiMatrix4x4 aiMat = bone.second.Matrix;
		XMFLOAT4X4 dxMat = {};

		dxMat.m[0][0] = aiMat.a1; dxMat.m[0][1] = aiMat.b1; dxMat.m[0][2] = aiMat.c1; dxMat.m[0][3] = aiMat.d1;
		dxMat.m[1][0] = aiMat.a2; dxMat.m[1][1] = aiMat.b2; dxMat.m[1][2] = aiMat.c2; dxMat.m[1][3] = aiMat.d2;
		dxMat.m[2][0] = aiMat.a3; dxMat.m[2][1] = aiMat.b3; dxMat.m[2][2] = aiMat.c3; dxMat.m[2][3] = aiMat.d3;
		dxMat.m[3][0] = aiMat.a4; dxMat.m[3][1] = aiMat.b4; dxMat.m[3][2] = aiMat.c4; dxMat.m[3][3] = aiMat.d4;

		boneMatrices.emplace_back(dxMat);
	}

	Renderer::SetBoneMatrix(boneMatrices);
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time1, const char* AnimationName2, const float& time2, float BlendRatio)
{
	//if (m_Animation.count(AnimationName1) == 0)return;
	//if (m_Animation.count(AnimationName2) == 0)return;
	//if (!m_Animation[AnimationName1]->HasAnimations())return;
	//if (!m_Animation[AnimationName2]->HasAnimations())return;

	////�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	//aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	//// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	//float TicksPerSecond1;
	//if (animation1->mTicksPerSecond != 0.0f)
	//{
	//	TicksPerSecond1 = static_cast<float>(animation1->mTicksPerSecond);
	//}
	//else
	//{
	//	TicksPerSecond1 = 60.0f; // �f�t�H���g�̃��[�g
	//}
	//float TimeInTicks1 = time1 * TicksPerSecond1;

	//// �A�j���[�V�������Ԃ����[�v������
	//float AnimationTime1 = static_cast<float>(fmod(TimeInTicks1, animation1->mDuration));


	////�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	//aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	//// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	//float TicksPerSecond2;
	//if (animation2->mTicksPerSecond != 0.0f)
	//{
	//	TicksPerSecond2 = static_cast<float>(animation2->mTicksPerSecond);
	//}
	//else
	//{
	//	TicksPerSecond2 = 60.0f; // �f�t�H���g�̃��[�g
	//}
	//float TimeInTicks2 = time2 * TicksPerSecond2;

	//// �A�j���[�V�������Ԃ����[�v������
	//float AnimationTime2 = static_cast<float>(fmod(TimeInTicks2, animation2->mDuration));

	//for (std::pair<const std::string, BONE>& pair : m_Bone)
	//{
	//	BONE* bone = &m_Bone[pair.first];

	//	aiNodeAnim* nodeAnim1 = nullptr;

	//	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	//	{
	//		if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
	//		{
	//			nodeAnim1 = animation1->mChannels[c];
	//			break;
	//		}
	//	}

	//	aiNodeAnim* nodeAnim2 = nullptr;

	//	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	//	{
	//		if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
	//		{
	//			nodeAnim2 = animation2->mChannels[c];
	//			break;
	//		}
	//	}

	//	int f;

	//	aiQuaternion rot1;
	//	aiVector3D pos1;

	//	if (nodeAnim1)
	//	{
	//		// �L�[�t���[���̑���
	//		unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
	//		unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

	//		// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
	//		f = 0;
	//		for (unsigned int i = 0; i < numRotKeys; i++)
	//		{
	//			if (AnimationTime1 < nodeAnim1->mRotationKeys[i].mTime)
	//			{
	//				break;
	//			}
	//			f = i;
	//		}
	//		rot1 = nodeAnim1->mRotationKeys[f].mValue;

	//		f = 0;
	//		for (unsigned int i = 0; i < numPosKeys; i++)
	//		{
	//			if (AnimationTime1 < nodeAnim1->mPositionKeys[i].mTime)
	//			{
	//				break;
	//			}
	//			f = i;
	//		}
	//		pos1 = nodeAnim1->mPositionKeys[f].mValue;
	//	}

	//	aiQuaternion rot2;
	//	aiVector3D pos2;

	//	if (nodeAnim2)
	//	{
	//		// �L�[�t���[���̑���
	//		unsigned int numRotKeys = nodeAnim2->mNumRotationKeys;
	//		unsigned int numPosKeys = nodeAnim2->mNumPositionKeys;

	//		// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
	//		f = 0;
	//		for (unsigned int i = 0; i < numRotKeys; i++)
	//		{
	//			if (AnimationTime2 < nodeAnim2->mRotationKeys[i].mTime)
	//			{
	//				break;
	//			}
	//			f = i;
	//		}
	//		rot2 = nodeAnim2->mRotationKeys[f].mValue;

	//		f = 0;
	//		for (unsigned int i = 0; i < numPosKeys; i++)
	//		{
	//			if (AnimationTime2 < nodeAnim2->mPositionKeys[i].mTime)
	//			{
	//				break;
	//			}
	//			f = i;
	//		}
	//		pos2 = nodeAnim2->mPositionKeys[f].mValue;
	//	}

	//	aiVector3D pos;
	//	pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;// ���`���
	//	aiQuaternion rot;
	//	aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);// ���ʐ��`���

	//	bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

	//}

	////�ċA�I�Ƀ{�[���}�g���N�X���X�V
	//aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	//UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	////���_�ϊ�(CPU�X�L�j���O)
	//for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	//{
	//	aiMesh* mesh = m_AiScene->mMeshes[m];

	//	D3D11_MAPPED_SUBRESOURCE ms;
	//	Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

	//	VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

	//	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	//	{
	//		DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

	//		aiMatrix4x4 matrix[4];
	//		aiMatrix4x4 outMatrix;
	//		matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
	//		matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
	//		matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
	//		matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

	//		{
	//			//�E�F�C�g���l�����ă}�g���N�X�Z�o
	//			outMatrix = matrix[0] * deformVertex->BoneWeight[0]
	//				+ matrix[1] * deformVertex->BoneWeight[1]
	//				+ matrix[2] * deformVertex->BoneWeight[2]
	//				+ matrix[3] * deformVertex->BoneWeight[3];


	//			outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a1 * deformVertex->BoneWeight[3];

	//			outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a2 * deformVertex->BoneWeight[3];

	//			outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a3 * deformVertex->BoneWeight[3];

	//			outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a4 * deformVertex->BoneWeight[3];


	//			outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d1 * deformVertex->BoneWeight[3];

	//			outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d2 * deformVertex->BoneWeight[3];

	//			outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d3 * deformVertex->BoneWeight[3];

	//			outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d4 * deformVertex->BoneWeight[3];
	//		}

	//		deformVertex->Position = mesh->mVertices[v];
	//		deformVertex->Position *= outMatrix;

	//		//�@���ϊ��p�Ɉړ��������폜
	//		outMatrix.a4 = 0.0f;
	//		outMatrix.b4 = 0.0f;
	//		outMatrix.c4 = 0.0f;

	//		deformVertex->Normal = mesh->mNormals[v];
	//		deformVertex->Normal *= outMatrix;

	//		//���_�o�b�t�@�֏�������
	//		vertex[v].Position.x = deformVertex->Position.x;
	//		vertex[v].Position.y = deformVertex->Position.y;
	//		vertex[v].Position.z = deformVertex->Position.z;

	//		vertex[v].Normal.x = deformVertex->Normal.x;
	//		vertex[v].Normal.y = deformVertex->Normal.y;
	//		vertex[v].Normal.z = deformVertex->Normal.z;

	//		vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
	//		vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

	//		vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	//}

}
void FbxModelRenderer::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;//������|���Ȃ��Ƃ����Ȃ�

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void FbxModelRenderer::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];


	// �{�[�����ƃC���f�b�N�X�̃}�b�v���쐬
	std::map<std::string, UINT> boneNameIndex;
	int boneCount = 0;

	// �{�[���̍쐬�ƃC���f�b�N�X�̊��蓖��
	CreateBone(m_AiScene->mRootNode, boneNameIndex, boneCount);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			// ���_�f�[�^�̏�����
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// �{�[���C���f�b�N�X�ƃE�F�C�g�̏�����
				for (int i = 0; i < 4; i++)
				{
					vertex[v].BoneIndices[i] = 0;
					vertex[v].BoneWeights[i] = 0.0f;
				}
			}

			// �{�[���f�[�^�̐ݒ�
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];
				const std::string& boneName = bone->mName.C_Str();

				// �{�[���C���f�b�N�X�̎擾
				UINT boneIndex = 0;
				auto it = boneNameIndex.find(boneName);
				if (it != boneNameIndex.end())
				{
					boneIndex = it->second;
				}
				else
				{
					boneIndex = boneCount++;
					boneNameIndex[boneName] = boneIndex;
				}

				m_Bone[boneName].OffsetMatrix = bone->mOffsetMatrix;

				//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];
					UINT vertexId = weight.mVertexId;
					float boneWeight = weight.mWeight;

					VERTEX_3D& v = vertex[vertexId];
					for (int i = 0; i < 4; i++)
					{
						if (v.BoneWeights[m] == 0.0f)
						{
							v.BoneIndices[m] = boneIndex;
							v.BoneWeights[m] = boneWeight;
							break;
						}
					}
				}

				//// �I�t�Z�b�g�}�g���N�X�̐ݒ�
				//m_Bone[boneName].OffsetMatrix = bone->mOffsetMatrix;

				//// �E�F�C�g�������_�Ƀ{�[������ݒ�
				//for (unsigned int w = 0; w < bone->mNumWeights; w++)
				//{
				//	aiVertexWeight weight = bone->mWeights[w];
				//	UINT vertexId = weight.mVertexId;
				//	float boneWeight = weight.mWeight;

				//	VERTEX_3D& v = vertex[vertexId];

				//	// �󂢂Ă���X���b�g�Ƀ{�[������ݒ�
				//	for (int i = 0; i < 4; i++)
				//	{
				//		if (v.BoneIndices[i] == 0)
				//		{
				//			v.BoneIndices[i] = boneIndex;
				//			v.BoneWeights[i] = boneWeight;
				//			break;
				//		}
				//	}
				//}
			}

			// ���_�o�b�t�@�̍쐬
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		// �ŏ��l�ƍő�l�̏�����
		XMFLOAT3 min = {};
		XMFLOAT3 max = {};

		// ���b�V���ƒ��_�𑖍�
		for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = m_AiScene->mMeshes[m];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				aiVector3D vertex = mesh->mVertices[v];

				// �ŏ��l�̍X�V
				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);
				min.z = std::min(min.z, vertex.z);

				// �ő�l�̍X�V
				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
				max.z = std::max(max.z, vertex.z);
			}
		}

		// ���S���W�̌v�Z
		const XMFLOAT3& center = { (min.x + max.x) / 2.0f ,(min.y+ max.y) / 2.0f ,(min.z + max.z) / 2.0f };
		m_Center = center;

		// �T�C�Y�̌v�Z
		const XMFLOAT3& size = { max.x - min.x ,max.y - min.y ,max.z - min.z };

		m_Scale = size;
	}



	//�e�N�X�`���ǂݍ���
	for (unsigned int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// �e�N�X�`���ǂݍ���
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}
}




void FbxModelRenderer::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(m_AiScene);

	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}

}


