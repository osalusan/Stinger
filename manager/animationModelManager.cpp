#include "animationModelManager.h"
#include "renderer/animationModelRenderer.h"

std::unordered_map<ANIMETION_MODEL, AnimationModelRenderer*> AnimationModelManager::m_LoadModelPool = {};
std::unordered_map<ANIMETION_MODEL, std::string> AnimationModelManager::m_ReservModelPool = {};
std::unordered_map<ANIMETION_MODEL, std::string> AnimationModelManager::m_ReservAnimationPool = {};
std::unordered_map<std::string, std::string> AnimationModelManager::m_ReservAnimationNamePool = {};

void AnimationModelManager::Init()
{
    // モデルの読み込み
    for (const std::pair<const ANIMETION_MODEL, std::string>& ReservModel : m_ReservModelPool)
    {
        LoadModel(ReservModel.first, ReservModel.second);
    }

    // アニメーションの読み込み
    for (const std::pair<const ANIMETION_MODEL, std::string>& ReservAnimation : m_ReservAnimationPool)
    {
        LoadAnimation(ReservAnimation.first, ReservAnimation.second, m_ReservAnimationNamePool[ReservAnimation.second]);
    }

    m_ReservModelPool.clear();
    m_ReservAnimationPool.clear();
    m_ReservAnimationNamePool.clear();
}

void AnimationModelManager::Uninit()
{
    m_ReservModelPool.clear();
    m_ReservAnimationPool.clear();
    m_ReservAnimationNamePool.clear();
    for (std::pair<ANIMETION_MODEL, AnimationModelRenderer*> LoadModel : m_LoadModelPool)
    {
        if (LoadModel.second == nullptr) continue;
        LoadModel.second->Uninit();

        delete  LoadModel.second;
        LoadModel.second = nullptr;
    }
    m_LoadModelPool.clear();
}

void AnimationModelManager::ReservModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // モデルの予約
    m_ReservModelPool.emplace(model, fileName);
}

void AnimationModelManager::ReservAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    // アニメーションの予約
    m_ReservAnimationPool.emplace(model, fileName);
    m_ReservAnimationNamePool.emplace(fileName, animationName);
}

AnimationModelRenderer* AnimationModelManager::GetAnimationModel(const ANIMETION_MODEL& model)
{
    return m_LoadModelPool[model];
}

// -------------------------- private --------------------------
void AnimationModelManager::LoadModel(const ANIMETION_MODEL& model, const std::string& fileName)
{
    // 保存済みのモデルがある場合
    if (m_LoadModelPool.count(model) > 0)
    {
        return; // 読み込まない
    }

    if (m_LoadModelPool[model] == nullptr)
    {
        m_LoadModelPool[model] = new AnimationModelRenderer;
    }
    
    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->Load(fileName.c_str());
    }
}

void AnimationModelManager::LoadAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName)
{
    // 保存済みのアニメーションがある場合
    if (m_LoadModelPool.count(model) > 0)
    {
        return; // 読み込まない
    }

    if (m_LoadModelPool[model] != nullptr)
    {
        m_LoadModelPool[model]->LoadAnimation(fileName.c_str(), animationName.c_str());
    }
}
