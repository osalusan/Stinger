#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "objModelManager.h"
#include "renderer/objModelRenderer.h"
#include "main/main.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

std::unordered_map<STATICMESH_MODEL, MODEL*> ObjModelManager::m_LoadModelPool = {};
std::unordered_map<STATICMESH_MODEL, std::string> ObjModelManager::m_ReservModelPool = {};

// ------------------------------- private -----------------------------------
void ObjModelManager::LoadModel(const char* FileName, MODEL* Model)
{
	MODEL_OBJ modelObj;
	LoadObj(FileName, &modelObj);



	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * modelObj.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->VertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * modelObj.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = modelObj.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		Model->SubsetArray = new SUBSET[modelObj.SubsetNum];
		Model->SubsetNum = modelObj.SubsetNum;

		for (unsigned int i = 0; i < modelObj.SubsetNum; i++)
		{
			Model->SubsetArray[i].StartIndex = modelObj.SubsetArray[i].StartIndex;
			Model->SubsetArray[i].IndexNum = modelObj.SubsetArray[i].IndexNum;

			Model->SubsetArray[i].Material.Material = modelObj.SubsetArray[i].Material.Material;

			Model->SubsetArray[i].Material.Texture = nullptr;

			// �e�N�X�`���ǂݍ���
			TexMetadata metadata;
			ScratchImage image;
			wchar_t wc[256] = { 0 };
			mbstowcs(wc, modelObj.SubsetArray[i].Material.TextureName, sizeof(wc));
			LoadFromWICFile(wc, WIC_FLAGS_NONE, &metadata, image);
			CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &Model->SubsetArray[i].Material.Texture);

			if (Model->SubsetArray[i].Material.Texture)
				Model->SubsetArray[i].Material.Material.TextureEnable = true;
			else
				Model->SubsetArray[i].Material.Material.TextureEnable = false;

		}
	}

	delete[] modelObj.VertexArray;
	delete[] modelObj.IndexArray;
	delete[] modelObj.SubsetArray;
}

void ObjModelManager::LoadObj(const char* FileName, MODEL_OBJ* ModelObj)
{
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);





	XMFLOAT3* positionArray = nullptr;
	XMFLOAT3* normalArray = nullptr;
	XMFLOAT2* texcoordArray = nullptr;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = nullptr;
	unsigned int	materialNum = 0;

	char str[256] = { "\0" };
	char* s = nullptr;
	char c = '\0';

	bool reverseX = false;
	bool reverseY = false;

	FILE* file = nullptr;
	file = fopen(FileName, "rt");
	assert(file);



	//�v�f���J�E���g
	while (true)
	{
		if (fscanf(file, "%s", str) == 0)
		{
			break;
		}

		if (feof(file) != 0)
		{
			break;
		}

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				if (fscanf(file, "%s", str) == 0)
				{
					break;
				}
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
				in = 6;

			indexNum += in;
		}
		else if (strcmp(str, "reverseX") == 0)
		{
			reverseX = true;
		}
		else if (strcmp(str, "reverseY") == 0)
		{
			reverseY = true;
		}
	}


	//�������m��
	positionArray = new XMFLOAT3[positionNum];
	normalArray = new XMFLOAT3[normalNum];
	texcoordArray = new XMFLOAT2[texcoordNum];


	ModelObj->VertexArray = new VERTEX_3D[vertexNum];
	ModelObj->VertexNum = vertexNum;

	ModelObj->IndexArray = new unsigned int[indexNum];
	ModelObj->IndexNum = indexNum;

	ModelObj->SubsetArray = new SUBSET[subsetNum];
	ModelObj->SubsetNum = subsetNum;




	//�v�f�Ǎ�
	XMFLOAT3* position = positionArray;
	XMFLOAT3* normal = normalArray;
	XMFLOAT2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek(file, 0, SEEK_SET);

	while (true)
	{
		if (fscanf(file, "%s", str) == 0)
		{
			break;
		}

		if (feof(file) != 0)
		{
			break;
		}

		if (strcmp(str, "mtllib") == 0)
		{
			//�}�e���A���t�@�C��
			if (fscanf(file, "%s", str) == 0)
			{
				break;
			}

			char path[256] = { "\0" };
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//�I�u�W�F�N�g��
			if (fscanf(file, "%s", str) == 0)
			{
				break;
			}
		}
		else if (strcmp(str, "v") == 0)
		{
			//���_���W
			if (fscanf(file, "%f", &position->x) == 0) break;
			if (fscanf(file, "%f", &position->y) == 0) break;
			if (fscanf(file, "%f", &position->z) == 0) break;
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//�@��
			if (fscanf(file, "%f", &normal->x) == 0) break;
			if (fscanf(file, "%f", &normal->y) == 0) break;
			if (fscanf(file, "%f", &normal->z) == 0) break;
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//�e�N�X�`�����W
			if (fscanf(file, "%f", &texcoord->x) == 0) break;
			if (fscanf(file, "%f", &texcoord->y) == 0) break;

			if (reverseX) { texcoord->x = 1.0f - texcoord->x; }
			if (reverseY) { texcoord->y = 1.0f - texcoord->y; }
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//�}�e���A��
			if (fscanf(file, "%s", str) == 0) break;

			if (sc != 0)
			{
				ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;
			}

			ModelObj->SubsetArray[sc].StartIndex = ic;


			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					ModelObj->SubsetArray[sc].Material.Material = materialArray[i].Material;
					strcpy(ModelObj->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(ModelObj->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0)
		{
			//��
			in = 0;

			do
			{
				if (fscanf(file, "%s", str) == 0)
				{
					break;
				}

				s = strtok(str, "/");
				ModelObj->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(nullptr, "/");
					ModelObj->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(nullptr, "/");
				ModelObj->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				ModelObj->VertexArray[vc].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				ModelObj->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
			{
				ModelObj->IndexArray[ic] = vc - 4;
				ic++;
				ModelObj->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;


	fclose(file);


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

void ObjModelManager::LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum)
{
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);

	char str[256] = { "\0" };

	FILE* file = nullptr;
	file = fopen(FileName, "rt");
	assert(file);

	MODEL_MATERIAL* materialArray = nullptr;
	unsigned int materialNum = 0;

	//�v�f���J�E���g
	while (true)
	{
		if (fscanf(file, "%s", str) == 0)
		{
			break;
		}

		if (feof(file) != 0)
		{
			break;
		}

		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new MODEL_MATERIAL[materialNum];

	if (materialArray == nullptr)
	{
		return;
	}
	//�v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		if (fscanf(file, "%s", str) == 0)
		{
			break;
		}

		if (feof(file) != 0)
		{
			break;
		}


		if (strcmp(str, "newmtl") == 0)
		{
			//�}�e���A����
			mc++;
			if (fscanf(file, "%s", materialArray[mc].Name) == 0)
			{
				break;
			}
			strcpy(materialArray[mc].TextureName, "");

			materialArray[mc].Material.Emission.x = 0.0f;
			materialArray[mc].Material.Emission.y = 0.0f;
			materialArray[mc].Material.Emission.z = 0.0f;
			materialArray[mc].Material.Emission.w = 0.0f;
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//�A���r�G���g
			if (fscanf(file, "%f", &materialArray[mc].Material.Ambient.x) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Ambient.y) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Ambient.z) == 0) break;
			materialArray[mc].Material.Ambient.w = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//�f�B�t���[�Y
			if (fscanf(file, "%f", &materialArray[mc].Material.Diffuse.x) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Diffuse.y) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Diffuse.z) == 0) break;
			materialArray[mc].Material.Diffuse.w = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//�X�y�L����
			if (fscanf(file, "%f", &materialArray[mc].Material.Specular.x) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Specular.y) == 0) break;
			if (fscanf(file, "%f", &materialArray[mc].Material.Specular.z) == 0) break;
			materialArray[mc].Material.Specular.w = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//�X�y�L�������x
			if (fscanf(file, "%f", &materialArray[mc].Material.Shininess) == 0) break;
		}
		else if (strcmp(str, "d") == 0)
		{
			//�A���t�@
			if (fscanf(file, "%f", &materialArray[mc].Material.Diffuse.w) == 0) break;
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//�e�N�X�`��
			if (fscanf(file, "%s", str) == 0) break;

			char path[256] = { "\0" };
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);
		}
	}


	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

void ObjModelManager::Load(const STATICMESH_MODEL& model, const std::string& fileName)
{
	if (m_LoadModelPool.count(model) > 0)
	{
		return;
	}

	MODEL* Model = new MODEL;
	LoadModel(fileName.c_str(), Model);

	m_LoadModelPool.emplace(model, Model);
}
// ------------------------------- public -----------------------------------

void ObjModelManager::Init()
{
	// ���f���̓ǂݍ���
	for (const std::pair<const STATICMESH_MODEL, std::string>& ReservModel : m_ReservModelPool)
	{
		Load(ReservModel.first, ReservModel.second);
	}

	m_ReservModelPool.clear();
}

void ObjModelManager::UnloadAll()
{
	for (std::pair<const STATICMESH_MODEL& , MODEL*> pair : m_LoadModelPool)
	{
		pair.second->VertexBuffer->Release();
		pair.second->IndexBuffer->Release();

		for (unsigned int i = 0; i < pair.second->SubsetNum; i++)
		{
			if (pair.second->SubsetArray[i].Material.Texture)
				pair.second->SubsetArray[i].Material.Texture->Release();
		}

		delete[] pair.second->SubsetArray;

		delete pair.second;
	}

	m_LoadModelPool.clear();
}

void ObjModelManager::ReservModel(const STATICMESH_MODEL& model, const std::string& fileName)
{
	m_ReservModelPool.emplace(model,fileName);
}

const MODEL* ObjModelManager::GetModel(const STATICMESH_MODEL& model)
{
	return m_LoadModelPool[model];
}
