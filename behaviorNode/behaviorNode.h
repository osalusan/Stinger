#pragma once
#include <string>
#include <vector>

enum class NODE_STATE 
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
    NODE_STATE m_CurrentState = NODE_STATE::FAILURE;
public:
    virtual ~BehaviorNode() {};
    virtual void Init() = 0;
    virtual NODE_STATE Update(const float& deltaTime) = 0;

    template <typename T, typename... Arg>
    BehaviorNode* AddTaskChild(Arg&&...args)
    {
        BehaviorNode* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return nullptr;
        node->Init();
        m_Children.emplace_back(node);
        return node;
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
    const NODE_STATE& GetNodeState()const
    {
        return m_CurrentState;
    }

    // ルートノード用
    void SetTaskName(const std::string& name)
    {
        m_TaskName = name;
    }
    // Nodeでのみ使用
    void SetCurrentState(const NODE_STATE& state)
    {
        m_CurrentState = state;
    }
};