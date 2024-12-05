#include "main/main.h"
#include "renderer/fbxModelRenderer.h"

void FbxModelRenderer::Draw()
{
	// GPUスキニング
	std::vector<XMFLOAT4X4> boneMatrix;
	for (const auto& bone : m_Bone)
	{
		aiMatrix4x4 aiMat = bone.second.Matrix;
		XMFLOAT4X4 mat = {};

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

		// 転置
		XMMATRIX matTranspose = XMMatrixTranspose(XMLoadFloat4x4(&mat));
		XMStoreFloat4x4(&mat, matTranspose);

		boneMatrix.emplace_back(mat);
	}
	// ボーン情報をシェーダーに
	Renderer::SetBoneMatrix(boneMatrix);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];


		// マテリアル設定
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

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void FbxModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void FbxModelRenderer::CreateBone(aiNode* node, std::map<std::string, int>& boneIndexMap, int& boneCount)
{

	// ボーン名を取得
	std::string boneName = node->mName.C_Str();

	BONE bone;
	m_Bone[boneName] = bone;

	// ボーンがまだマップに存在しない場合、インデックスを割り当てる
	if (boneIndexMap.count(boneName) <= 0)
	{
		boneIndexMap[boneName] = boneCount;
		boneCount++;
	}

	// 子ノードを再帰的に処理
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		CreateBone(node->mChildren[i], boneIndexMap, boneCount);
	}
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time)
{
	if (m_Animation.count(AnimationName1) == 0)return;
	if (!m_Animation[AnimationName1]->HasAnimations())return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	// アニメーションのティック単位の時間を計算
	float TicksPerSecond;
	if (animation1->mTicksPerSecond != 0.0f)
	{
		TicksPerSecond = static_cast<float>(animation1->mTicksPerSecond);
	}
	else
	{
		TicksPerSecond = 60.0f; // デフォルトのティックレート
	}
	float TimeInTicks = time * TicksPerSecond;

	// アニメーション時間をループさせる
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
			// キーフレームの総数
			unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
			unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

			// キーフレームのインデックスを計算（補間なし）
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

	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	//頂点変換(CPUスキニング / Vertex)
	{
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
		//		
		//		VERTEX_3D& vIn = vertex[v];
		//		
		//		matrix[0] = m_Bone[m_NameList[vIn.BoneIndices[0]]].Matrix;
		//		matrix[1] = m_Bone[m_NameList[vIn.BoneIndices[1]]].Matrix;
		//		matrix[2] = m_Bone[m_NameList[vIn.BoneIndices[2]]].Matrix;
		//		matrix[3] = m_Bone[m_NameList[vIn.BoneIndices[3]]].Matrix;


		//		{
		//			//ウェイトを考慮してマトリクス算出
		//			outMatrix = matrix[0] * vIn.BoneWeights[0]
		//				+ matrix[1] * vIn.BoneWeights[1]
		//				+ matrix[2] * vIn.BoneWeights[2]
		//				+ matrix[3] * vIn.BoneWeights[3];
		//		}

		//		deformVertex->Position = mesh->mVertices[v];
		//		//deformVertex->Position *= outMatrix;


		//		//// 行列とベクトルの積を計算
		//		deformVertex->Position.x = mesh->mVertices[v].x * outMatrix.a1 +
		//			mesh->mVertices[v].y * outMatrix.a2 +
		//			mesh->mVertices[v].z * outMatrix.a3 + outMatrix.a4;

		//		deformVertex->Position.y = mesh->mVertices[v].x * outMatrix.b1 +
		//			mesh->mVertices[v].y * outMatrix.b2 +
		//			mesh->mVertices[v].z * outMatrix.b3 + outMatrix.b4;

		//		deformVertex->Position.z = mesh->mVertices[v].x * outMatrix.c1 +
		//			mesh->mVertices[v].y * outMatrix.c2 +
		//			mesh->mVertices[v].z * outMatrix.c3 + outMatrix.c4;

		//		//法線変換用に移動成分を削除
		//		outMatrix.a4 = 0.0f;
		//		outMatrix.b4 = 0.0f;
		//		outMatrix.c4 = 0.0f;

		//		deformVertex->Normal = mesh->mNormals[v];
		//		deformVertex->Normal *= outMatrix;

		//		//頂点バッファへ書き込み
		//		vIn.Position.x = deformVertex->Position.x;
		//		vIn.Position.y = deformVertex->Position.y;
		//		vIn.Position.z = deformVertex->Position.z;

		//		vIn.Normal.x = deformVertex->Normal.x;
		//		vIn.Normal.y = deformVertex->Normal.y;
		//		vIn.Normal.z = deformVertex->Normal.z;

		//		vIn.TexCoord.x = mesh->mTextureCoords[0][v].x;
		//		vIn.TexCoord.y = mesh->mTextureCoords[0][v].y;

		//		vIn.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//	}
		//	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
		//}
	}
	

	//頂点変換(CPUスキニング)
	{
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
		//		matrix[0] = m_Bone[m_NameList[deformVertex->BoneIndex[0]]].Matrix;
		//		matrix[1] = m_Bone[m_NameList[deformVertex->BoneIndex[1]]].Matrix;
		//		matrix[2] = m_Bone[m_NameList[deformVertex->BoneIndex[2]]].Matrix;
		//		matrix[3] = m_Bone[m_NameList[deformVertex->BoneIndex[3]]].Matrix;

		//		{
		//			//ウェイトを考慮してマトリクス算出
		//			outMatrix = matrix[0] * deformVertex->BoneWeight[0]
		//				+ matrix[1] * deformVertex->BoneWeight[1]
		//				+ matrix[2] * deformVertex->BoneWeight[2]
		//				+ matrix[3] * deformVertex->BoneWeight[3];

		//		}

		//		deformVertex->Position = mesh->mVertices[v];
		//		deformVertex->Position *= outMatrix;

		//		//法線変換用に移動成分を削除
		//		outMatrix.a4 = 0.0f;
		//		outMatrix.b4 = 0.0f;
		//		outMatrix.c4 = 0.0f;

		//		deformVertex->Normal = mesh->mNormals[v];
		//		deformVertex->Normal *= outMatrix;

		//		//頂点バッファへ書き込み
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
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time1, const char* AnimationName2, const float& time2, float BlendRatio)
{
	//if (m_Animation.count(AnimationName1) == 0)return;
	//if (m_Animation.count(AnimationName2) == 0)return;
	//if (!m_Animation[AnimationName1]->HasAnimations())return;
	//if (!m_Animation[AnimationName2]->HasAnimations())return;

	////アニメーションデータからボーンマトリクス算出
	//aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	//// アニメーションのティック単位の時間を計算
	//float TicksPerSecond1;
	//if (animation1->mTicksPerSecond != 0.0f)
	//{
	//	TicksPerSecond1 = static_cast<float>(animation1->mTicksPerSecond);
	//}
	//else
	//{
	//	TicksPerSecond1 = 60.0f; // デフォルトのレート
	//}
	//float TimeInTicks1 = time1 * TicksPerSecond1;

	//// アニメーション時間をループさせる
	//float AnimationTime1 = static_cast<float>(fmod(TimeInTicks1, animation1->mDuration));


	////アニメーションデータからボーンマトリクス算出
	//aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	//// アニメーションのティック単位の時間を計算
	//float TicksPerSecond2;
	//if (animation2->mTicksPerSecond != 0.0f)
	//{
	//	TicksPerSecond2 = static_cast<float>(animation2->mTicksPerSecond);
	//}
	//else
	//{
	//	TicksPerSecond2 = 60.0f; // デフォルトのレート
	//}
	//float TimeInTicks2 = time2 * TicksPerSecond2;

	//// アニメーション時間をループさせる
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
	//		// キーフレームの総数
	//		unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
	//		unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

	//		// キーフレームのインデックスを計算（補間なし）
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
	//		// キーフレームの総数
	//		unsigned int numRotKeys = nodeAnim2->mNumRotationKeys;
	//		unsigned int numPosKeys = nodeAnim2->mNumPositionKeys;

	//		// キーフレームのインデックスを計算（補間なし）
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
	//	pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;// 線形補間
	//	aiQuaternion rot;
	//	aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);// 球面線形補間

	//	bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

	//}

	////再帰的にボーンマトリクスを更新
	//aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	//UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	////頂点変換(CPUスキニング)
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
	//			//ウェイトを考慮してマトリクス算出
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

	//		//法線変換用に移動成分を削除
	//		outMatrix.a4 = 0.0f;
	//		outMatrix.b4 = 0.0f;
	//		outMatrix.c4 = 0.0f;

	//		deformVertex->Normal = mesh->mNormals[v];
	//		deformVertex->Normal *= outMatrix;

	//		//頂点バッファへ書き込み
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

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;//これを掛けないといけない

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

	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	// ボーン名とインデックスのマップを作成
	std::map<std::string, int> boneNameIndex;
	int boneCount = 0;

	// ボーンの作成とインデックスの割り当て
	CreateBone(m_AiScene->mRootNode, boneNameIndex, boneCount);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			// 頂点データの初期化
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// ボーンインデックスとウェイトの初期化
				for (int i = 0; i < 4; i++)
				{
					vertex[v].BoneIndices[i] = 0;
					vertex[v].BoneWeights[i] = 0.0f;
				}

				DEFORM_VERTEX deformVertex;
				deformVertex.Position = mesh->mVertices[v];
				deformVertex.Normal = mesh->mNormals[v];
				deformVertex.BoneNum = 0;

				for (unsigned int b = 0; b < 4; b++)
				{
					deformVertex.BoneIndex[b] = 0;
					deformVertex.BoneWeight[b] = 0.0f;
				}

				m_DeformVertex[m].push_back(deformVertex);
			}

			// ボーンデータの設定
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];
				const std::string& boneName = bone->mName.C_Str();

				// ボーンインデックスの取得
				const int& boneIndex = boneNameIndex[boneName];

				m_NameList.emplace(boneIndex, boneName);

				m_Bone[boneName].OffsetMatrix = bone->mOffsetMatrix;

				
				//変形後頂点にボーンデータ格納
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];
					int vertexId = weight.mVertexId;
					float boneWeight = weight.mWeight;

					VERTEX_3D& v = vertex[vertexId];

					for (int i = 0; i < 4; i++)
					{
						if (v.BoneWeights[i] == 0.0f)
						{
							v.BoneIndices[i] = boneIndex;
							v.BoneWeights[i] = boneWeight;
							break;
						}
					}

					for (int i = 0; i < 4; i++)
					{
						if (m_DeformVertex[m][vertexId].BoneIndex[i] == 0.0f)
						{
							m_DeformVertex[m][vertexId].BoneWeight[i] = boneWeight;
							m_DeformVertex[m][vertexId].BoneIndex[i] = boneIndex;
							break;
						}
					}

					assert(m_DeformVertex[m][vertexId].BoneNum <= 4);
				}
			}

			// ウェイトの正規化
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				VERTEX_3D& vert = vertex[v];

				// ウェイトの合計を計算
				float totalWeight = vert.BoneWeights[0] + vert.BoneWeights[1] + vert.BoneWeights[2] + vert.BoneWeights[3];

				// ウェイトの合計が0でない場合のみ正規化
				if (totalWeight > 0.0f && totalWeight != 1.0f)
				{
					vert.BoneWeights[0] /= totalWeight;
					vert.BoneWeights[1] /= totalWeight;
					vert.BoneWeights[2] /= totalWeight;
					vert.BoneWeights[3] /= totalWeight;
				}
			}



			// 頂点バッファの作成
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
		}


		// インデックスバッファ生成
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

		// 最小値と最大値の初期化
		XMFLOAT3 min = {};
		XMFLOAT3 max = {};

		// メッシュと頂点を走査
		for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = m_AiScene->mMeshes[m];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				aiVector3D vertex = mesh->mVertices[v];

				// 最小値の更新
				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);
				min.z = std::min(min.z, vertex.z);

				// 最大値の更新
				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
				max.z = std::max(max.z, vertex.z);
			}
		}

		// 中心座標の計算
		const XMFLOAT3& center = { (min.x + max.x) / 2.0f ,(min.y+ max.y) / 2.0f ,(min.z + max.z) / 2.0f };
		m_Center = center;

		// サイズの計算
		const XMFLOAT3& size = { max.x - min.x ,max.y - min.y ,max.z - min.z };

		m_Scale = size;
	}



	//テクスチャ読み込み
	for (unsigned int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// テクスチャ読み込み
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

	delete[] m_DeformVertex;

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


