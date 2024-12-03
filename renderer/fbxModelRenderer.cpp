#include "main/main.h"
#include "renderer/fbxModelRenderer.h"

void FbxModelRenderer::Draw()
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

		// 頂点バッファの設定（頂点バッファを使用する場合）
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffers[m], &stride, &offset);

		// インデックスバッファの設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(indexBuffers[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
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
		TicksPerSecond = 25.0f; // デフォルトのティックレート
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

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
	}

	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// GPUスキニングを行う (ComputeShaderに処理を委譲している)
	Renderer::GetDeviceContext()->CSSetShader(_skinCS, nullptr, 0);

	for (size_t m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		// メッシュごとのデータを取得
		aiMesh* mesh = m_AiScene->mMeshes[m];
		ID3D11Buffer* cb = constantBuffers[m];
		ID3D11ShaderResourceView* notDeformSRV = notDeformVertexSRVs[m];
		ID3D11UnorderedAccessView* uav = vertexUAVs[m];

		// Compute Shaderへのデータ送信
		Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &cb);
		Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &notDeformSRV);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

		// スレッドグループの計算
		UINT vertexCount = mesh->mNumVertices;
		UINT numThreadGroup = (vertexCount + 255) / 256;
		Renderer::GetDeviceContext()->Dispatch(numThreadGroup, 1, 1);

		// UAVのバインド解除
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

		// 頂点バッファ生成
		{
			VERTEX_3D* notDeformVertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				notDeformVertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				notDeformVertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				notDeformVertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				notDeformVertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// バッファ作成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bd.StructureByteStride = sizeof(VERTEX_3D);  // 各要素のサイズを設定

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = notDeformVertex;

			ID3D11Buffer* notDeformVB = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &notDeformVB);
			notDeformVertexBuffers.emplace_back(notDeformVB);

			delete[] notDeformVertex;

			// csで使われるsrvの作成
			D3D11_SHADER_RESOURCE_VIEW_DESC notDeformSRVDesc;
			ZeroMemory(&notDeformSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			notDeformSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			notDeformSRVDesc.Buffer.NumElements = mesh->mNumVertices;// 頂点数
			notDeformSRVDesc.Buffer.FirstElement = 0;
			notDeformSRVDesc.Format = DXGI_FORMAT_UNKNOWN;

			ID3D11ShaderResourceView* notDeformVertexSRV;
			Renderer::GetDevice()->CreateShaderResourceView(notDeformVB, &notDeformSRVDesc, &notDeformVertexSRV);
			notDeformVertexSRVs.emplace_back(notDeformVertexSRV);
		}

		

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// バッファ作成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bd.StructureByteStride = sizeof(VERTEX_3D);  // 各要素のサイズを設定

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			ID3D11Buffer* vb = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &vb);
			vertexBuffers.emplace_back(vb);

			// csで書き出される用のUAV
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = mesh->mNumVertices;// 頂点数

			ID3D11UnorderedAccessView* uav = nullptr;
			HRESULT hr = Renderer::GetDevice()->CreateUnorderedAccessView(vb, &uavDesc, &uav);
			vertexUAVs.emplace_back(uav);

			// vsで読み込まれるスキニング処理後を送るSRV
			D3D11_SHADER_RESOURCE_VIEW_DESC updatedSrvDesc = {};
			updatedSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			updatedSrvDesc.Buffer.FirstElement = 0;
			updatedSrvDesc.Buffer.NumElements = mesh->mNumVertices;
			updatedSrvDesc.Format = DXGI_FORMAT_UNKNOWN;

			ID3D11ShaderResourceView* srv = nullptr;
			Renderer::GetDevice()->CreateShaderResourceView(vb, &updatedSrvDesc, &srv);
			updatedSRVs.emplace_back(srv);

			// CSに渡す定数バッファの作成
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

			ID3D11Buffer* ib = nullptr;
			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &ib);
			indexBuffers.push_back(ib);

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

	Renderer::CreateComputeShader(_skinCS, "cso\\skinningCS.cso");
}




void FbxModelRenderer::Uninit()
{
	// インデックスバッファの解放
	for (size_t m = 0; m < indexBuffers.size(); m++)
	{
		if (indexBuffers[m])
		{
			indexBuffers[m]->Release();
			indexBuffers[m] = nullptr;
		}
	}
	indexBuffers.clear();

	// 頂点バッファの解放
	for (size_t m = 0; m < vertexBuffers.size(); m++)
	{
		if (vertexBuffers[m])
		{
			vertexBuffers[m]->Release();
			vertexBuffers[m] = nullptr;
		}
	}
	vertexBuffers.clear();

	// notDeformVertexBuffersの解放
	for (size_t m = 0; m < notDeformVertexBuffers.size(); m++)
	{
		if (notDeformVertexBuffers[m])
		{
			notDeformVertexBuffers[m]->Release();
			notDeformVertexBuffers[m] = nullptr;
		}
	}
	notDeformVertexBuffers.clear();

	// UAVの解放
	for (size_t m = 0; m < vertexUAVs.size(); m++)
	{
		if (vertexUAVs[m])
		{
			vertexUAVs[m]->Release();
			vertexUAVs[m] = nullptr;
		}
	}
	vertexUAVs.clear();

	// updatedSRVsの解放
	for (size_t m = 0; m < updatedSRVs.size(); m++)
	{
		if (updatedSRVs[m])
		{
			updatedSRVs[m]->Release();
			updatedSRVs[m] = nullptr;
		}
	}
	updatedSRVs.clear();

	// notDeformVertexSRVsの解放
	for (size_t m = 0; m < notDeformVertexSRVs.size(); m++)
	{
		if (notDeformVertexSRVs[m])
		{
			notDeformVertexSRVs[m]->Release();
			notDeformVertexSRVs[m] = nullptr;
		}
	}
	notDeformVertexSRVs.clear();

	// 定数バッファの解放
	for (size_t m = 0; m < constantBuffers.size(); m++)
	{
		if (constantBuffers[m])
		{
			constantBuffers[m]->Release();
			constantBuffers[m] = nullptr;
		}
	}
	constantBuffers.clear();

	// シーンデータの解放
	if (m_AiScene)
	{
		aiReleaseImport(m_AiScene);
		m_AiScene = nullptr;
	}

	// Compute Shaderの解放
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


