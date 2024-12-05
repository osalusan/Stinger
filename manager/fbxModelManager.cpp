#include "fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

std::unordered_map<ANIMETION_MODEL, FbxModelRenderer*> FbxModelManager::m_LoadModelPool = {};
std::unordered_map<ANIMETION_MODEL, std::string> FbxModelManager::m_ReservModelPool = {};
std::unordered_map<std::string, ANIMETION_MODEL> FbxModelManager::m_ReservAnimationPool = {};
std::unordered_map<std::string, std::string> FbxModelManager::m_ReservAnimationNamePool = {};

void FbxModelManager::Init()
{
    // モデルの読み込み
    for (const std::pair<const ANIMETION_MODEL, std::string>& reservModel : m_ReservModelPool)
    {
        LoadModel(reservModel.first, reservModel.second);
    }

    // アニメーションの読み込み
    for (const std::pair<const std::string, ANIMETION_MODEL>& reservAnimation : m_ReservAnimationPool)
    {
        LoadAnimation(reservAnimation.second, reservAnimation.first, m_ReservAnimationNamePool[reservAnimation.first]);
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
    for (std::pair<ANIMETION_MODEL, FbxModelRenderer*> loadModel : m_LoadModelPool)
    {
        if (loadModel.second == nullptr) continue;
        loadModel.second->Uninit();

        delete loadModel.second;
        loadModel.second = nullptr;
    }
    m_LoadModelPool.clear();
}

void FbxModelManager::ReservModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // モデルの予約
    m_ReservModelPool.emplace(model, fileName);
}

void FbxModelManager::ReservAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    // アニメーションの予約
    m_ReservAnimationPool.emplace(fileName, model);
    m_ReservAnimationNamePool.emplace(fileName, animationName);
}

FbxModelRenderer* FbxModelManager::GetAnimationModel(const ANIMETION_MODEL& model)
{
    return m_LoadModelPool[model];
}

// -------------------------- private --------------------------
void FbxModelManager::LoadModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // 保存済みのモデルがある場合
    if (m_LoadModelPool.count(model) > 0)
    {
        return; // 読み込まない
    }

    m_LoadModelPool.emplace(model, new FbxModelRenderer);
    
    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->Load(fileName.c_str());
    }
}

void FbxModelManager::LoadAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->LoadAnimation(fileName.c_str(), animationName.c_str());
    }
}
