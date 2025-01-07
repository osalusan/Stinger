#pragma once
#include <string>
#include <vector>

enum class NODE_STATUS 
{
    SUCCESS = 0,
    FAILURE,
    RUNNING
};

class BehaviorNode 
{
protected:
    std::string m_TaskName = {};
    std::vector<BehaviorNode*> m_Children = {};

public:
    virtual ~BehaviorNode() {};
    virtual void Init() = 0;
    virtual NODE_STATUS Update(const float& deltaTime) = 0;

    template <typename T, typename... Arg>
    void AddTaskChild(Arg&&...args)
    {
        T* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(node);
    }

    template <typename T>
    BehaviorNode* AddNodeChild(const std::string& name)
    {
        BehaviorNode* node = new T;
        if (node != nullptr)
        {
            node->SetTaskName(name);
            node->Init();
            m_Children.emplace_back(node);
        }
        return node;
    }

    const std::vector<BehaviorNode*> GetChildren()const
    {
        return m_Children;
    }

    const std::string GetTaskName()const
    {
        return m_TaskName;
    }

    // ルートノード用
    void SetTaskName(const std::string& name)
    {
        m_TaskName = name;
    }
};