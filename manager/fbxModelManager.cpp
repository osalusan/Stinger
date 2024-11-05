#include "fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

std::unordered_map<ANIMETION_MODEL, FbxModelRenderer*> FbxModelManager::m_LoadModelPool = {};
std::unordered_map<ANIMETION_MODEL, std::string> FbxModelManager::m_ReservModelPool = {};
std::unordered_map<ANIMETION_MODEL, std::string> FbxModelManager::m_ReservAnimationPool = {};
std::unordered_map<std::string, std::string> FbxModelManager::m_ReservAnimationNamePool = {};

void FbxModelManager::Init()
{
    // ���f���̓ǂݍ���
    for (const std::pair<const ANIMETION_MODEL, std::string>& ReservModel : m_ReservModelPool)
    {
        LoadModel(ReservModel.first, ReservModel.second);
    }

    // �A�j���[�V�����̓ǂݍ���
    for (const std::pair<const ANIMETION_MODEL, std::string>& ReservAnimation : m_ReservAnimationPool)
    {
        LoadAnimation(ReservAnimation.first, ReservAnimation.second, m_ReservAnimationNamePool[ReservAnimation.second]);
    }

    m_ReservModelPool.clear();
    m_ReservAnimationPool.clear();
    m_ReservAnimationNamePool.clear();
}

void FbxModelManager::Uninit()
{
    m_ReservModelPool.clear();
    m_ReservAnimationPool.clear();
    m_ReservAnimationNamePool.clear();
    for (std::pair<ANIMETION_MODEL, FbxModelRenderer*> LoadModel : m_LoadModelPool)
    {
        if (LoadModel.second == nullptr) continue;
        LoadModel.second->Uninit();

        delete  LoadModel.second;
        LoadModel.second = nullptr;
    }
    m_LoadModelPool.clear();
}

void FbxModelManager::ReservModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // ���f���̗\��
    m_ReservModelPool.emplace(model, fileName);
}

void FbxModelManager::ReservAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    // �A�j���[�V�����̗\��
    m_ReservAnimationPool.emplace(model, fileName);
    m_ReservAnimationNamePool.emplace(fileName, animationName);
}

FbxModelRenderer* FbxModelManager::GetAnimationModel(const ANIMETION_MODEL& model)
{
    return m_LoadModelPool[model];
}

// -------------------------- private --------------------------
void FbxModelManager::LoadModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // �ۑ��ς݂̃��f��������ꍇ
    if (m_LoadModelPool.count(model) > 0)
    {
        return; // �ǂݍ��܂Ȃ�
    }

    m_LoadModelPool.emplace(model, new FbxModelRenderer);
    
    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->Load(fileName.c_str());
    }
}

void FbxModelManager::LoadAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    // �ۑ��ς݂̃A�j���[�V����������ꍇ
    if (m_LoadModelPool.count(model) > 0)
    {
        return; // �ǂݍ��܂Ȃ�
    }

    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->LoadAnimation(fileName.c_str(), animationName.c_str());
    }
}
