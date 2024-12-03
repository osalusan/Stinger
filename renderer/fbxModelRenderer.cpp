#include "main/main.h"
#include "renderer/fbxModelRenderer.h"

void FbxModelRenderer::Draw()
{
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

		// ���_�o�b�t�@�̐ݒ�i���_�o�b�t�@���g�p����ꍇ�j
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffers[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(indexBuffers[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_AiScene->mMeshes[m]->mNumFaces * 3, 0, 0);
	}
	
}

void FbxModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void FbxModelRenderer::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
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
		TicksPerSecond = 25.0f; // �f�t�H���g�̃e�B�b�N���[�g
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

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// GPU�X�L�j���O���s�� (ComputeShader�ɏ������Ϗ����Ă���)
	Renderer::GetDeviceContext()->CSSetShader(_skinCS, nullptr, 0);

	for (size_t m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		// ���b�V�����Ƃ̃f�[�^���擾
		aiMesh* mesh = m_AiScene->mMeshes[m];
		ID3D11Buffer* cb = constantBuffers[m];
		ID3D11ShaderResourceView* notDeformSRV = notDeformVertexSRVs[m];
		ID3D11UnorderedAccessView* uav = vertexUAVs[m];

		// Compute Shader�ւ̃f�[�^���M
		Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &cb);
		Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &notDeformSRV);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

		// �X���b�h�O���[�v�̌v�Z
		UINT vertexCount = mesh->mNumVertices;
		UINT numThreadGroup = (vertexCount + 255) / 256;
		Renderer::GetDeviceContext()->Dispatch(numThreadGroup, 1, 1);

		// UAV�̃o�C���h����
		ID3D11UnorderedAccessView* nullUAV = nullptr;
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	}
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

	CreateBone(m_AiScene->mRootNode);

	totalVertexCount = 0;
	for (unsigned int i = 0; i < m_AiScene->mNumMeshes; i++) 
	{
		aiMesh* mesh = m_AiScene->mMeshes[i];
		totalVertexCount += mesh->mNumVertices;
	}

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* notDeformVertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				notDeformVertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				notDeformVertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				notDeformVertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				notDeformVertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �o�b�t�@�쐬
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bd.StructureByteStride = sizeof(VERTEX_3D);  // �e�v�f�̃T�C�Y��ݒ�

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = notDeformVertex;

			ID3D11Buffer* notDeformVB = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &notDeformVB);
			notDeformVertexBuffers.emplace_back(notDeformVB);

			delete[] notDeformVertex;

			// cs�Ŏg����srv�̍쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC notDeformSRVDesc;
			ZeroMemory(&notDeformSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			notDeformSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			notDeformSRVDesc.Buffer.NumElements = mesh->mNumVertices;// ���_��
			notDeformSRVDesc.Buffer.FirstElement = 0;
			notDeformSRVDesc.Format = DXGI_FORMAT_UNKNOWN;

			ID3D11ShaderResourceView* notDeformVertexSRV;
			Renderer::GetDevice()->CreateShaderResourceView(notDeformVB, &notDeformSRVDesc, &notDeformVertexSRV);
			notDeformVertexSRVs.emplace_back(notDeformVertexSRV);
		}

		

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �o�b�t�@�쐬
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bd.StructureByteStride = sizeof(VERTEX_3D);  // �e�v�f�̃T�C�Y��ݒ�

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			ID3D11Buffer* vb = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &vb);
			vertexBuffers.emplace_back(vb);

			// cs�ŏ����o�����p��UAV
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = mesh->mNumVertices;// ���_��

			ID3D11UnorderedAccessView* uav = nullptr;
			HRESULT hr = Renderer::GetDevice()->CreateUnorderedAccessView(vb, &uavDesc, &uav);
			vertexUAVs.emplace_back(uav);

			// vs�œǂݍ��܂��X�L�j���O������𑗂�SRV
			D3D11_SHADER_RESOURCE_VIEW_DESC updatedSrvDesc = {};
			updatedSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			updatedSrvDesc.Buffer.FirstElement = 0;
			updatedSrvDesc.Buffer.NumElements = mesh->mNumVertices;
			updatedSrvDesc.Format = DXGI_FORMAT_UNKNOWN;

			ID3D11ShaderResourceView* srv = nullptr;
			Renderer::GetDevice()->CreateShaderResourceView(vb, &updatedSrvDesc, &srv);
			updatedSRVs.emplace_back(srv);

			// CS�ɓn���萔�o�b�t�@�̍쐬
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.ByteWidth = sizeof(ANIMATIONBUFF);
			bufferDesc.StructureByteStride = 0;
			bufferDesc.MiscFlags = 0;

			ID3D11Buffer* cb = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cb);
			constantBuffers.push_back(cb);
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

			ID3D11Buffer* ib = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &ib);
			indexBuffers.push_back(ib);

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

	Renderer::CreateComputeShader(_skinCS, "cso\\skinningCS.cso");
}




void FbxModelRenderer::Uninit()
{
	// �C���f�b�N�X�o�b�t�@�̉��
	for (size_t m = 0; m < indexBuffers.size(); m++)
	{
		if (indexBuffers[m])
		{
			indexBuffers[m]->Release();
			indexBuffers[m] = nullptr;
		}
	}
	indexBuffers.clear();

	// ���_�o�b�t�@�̉��
	for (size_t m = 0; m < vertexBuffers.size(); m++)
	{
		if (vertexBuffers[m])
		{
			vertexBuffers[m]->Release();
			vertexBuffers[m] = nullptr;
		}
	}
	vertexBuffers.clear();

	// notDeformVertexBuffers�̉��
	for (size_t m = 0; m < notDeformVertexBuffers.size(); m++)
	{
		if (notDeformVertexBuffers[m])
		{
			notDeformVertexBuffers[m]->Release();
			notDeformVertexBuffers[m] = nullptr;
		}
	}
	notDeformVertexBuffers.clear();

	// UAV�̉��
	for (size_t m = 0; m < vertexUAVs.size(); m++)
	{
		if (vertexUAVs[m])
		{
			vertexUAVs[m]->Release();
			vertexUAVs[m] = nullptr;
		}
	}
	vertexUAVs.clear();

	// updatedSRVs�̉��
	for (size_t m = 0; m < updatedSRVs.size(); m++)
	{
		if (updatedSRVs[m])
		{
			updatedSRVs[m]->Release();
			updatedSRVs[m] = nullptr;
		}
	}
	updatedSRVs.clear();

	// notDeformVertexSRVs�̉��
	for (size_t m = 0; m < notDeformVertexSRVs.size(); m++)
	{
		if (notDeformVertexSRVs[m])
		{
			notDeformVertexSRVs[m]->Release();
			notDeformVertexSRVs[m] = nullptr;
		}
	}
	notDeformVertexSRVs.clear();

	// �萔�o�b�t�@�̉��
	for (size_t m = 0; m < constantBuffers.size(); m++)
	{
		if (constantBuffers[m])
		{
			constantBuffers[m]->Release();
			constantBuffers[m] = nullptr;
		}
	}
	constantBuffers.clear();

	// �V�[���f�[�^�̉��
	if (m_AiScene)
	{
		aiReleaseImport(m_AiScene);
		m_AiScene = nullptr;
	}

	// Compute Shader�̉��
	if (_skinCS)
	{
		_skinCS->Release();
		_skinCS = nullptr;
	}
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


