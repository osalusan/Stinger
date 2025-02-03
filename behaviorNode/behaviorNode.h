#pragma once
#include <string>
#include <vector>

enum class NODE_STATE 
{
    SUCCESS = 0,
    FAILURE,
    RUNNING
};

struct DERIVATION_DATA
{
    float Health = 0.0f;                 // ”h¶‰Â”\‘Ì—Í
    int Chance = 0;                      // ”h¶‹Z”­¶Šm—¦
    float TransTimeValue = 0.0f;	     // ”h¶ˆÚsŠÔŠ„‡
};

class BehaviorNode 
{
private:
    std::vector<DERIVATION_DATA> m_DerivationData = {};              // ”h¶‹Z‚Ìƒf[ƒ^‚ğŠi”[ / Get‚©‚ç‚µ‚©æ“¾‚Å‚«‚È‚¢‚æ‚¤‚É
protected:
    std::string m_TaskName = {};                                // CSV‚Ì•û‚Æ–¼‘O‚ğ“¯‚¶‚É
    std::vector<BehaviorNode*> m_Children = {};
    NODE_STATE m_CurrentState = NODE_STATE::FAILURE;

    std::vector<int> m_ChildDerivChance = {};		            // ”h¶‹Z‚ÌŠm—¦ / ‡Œv‚Å100‚É‚È‚é‚æ‚¤‚É
public:
    virtual ~BehaviorNode();
    virtual void Init() = 0;
    virtual NODE_STATE Update(const float& deltaTime) = 0;

    // –â‘è‚È‚©‚Á‚½‚çtrue‚ª•Ô‚é
    bool CheckRunningNode(BehaviorNode* currentNode);

    template <typename T, typename... Arg>
    BehaviorNode* AddTaskChild(Arg&&...args)
    {
        BehaviorNode* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return nullptr;
        node->Init();
        m_Children.emplace_back(node);
        return node;
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(int derivChance, Args&&... args)
    {
        m_ChildDerivChance.emplace_back(derivChance);
        
        return AddTaskChild<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(DERIVATION_DATA derivData, Args&&... args)
    {
        m_DerivationData.emplace_back(derivData);

        return AddTaskChild<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(int derivChance, DERIVATION_DATA derivData, Args&&... args)
    {
        m_ChildDerivChance.emplace_back(derivChance);
        m_DerivationData.emplace_back(derivData);

        return AddTaskChild<T>(std::forward<Args>(args)...);
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
    DERIVATION_DATA GetDerivationData(const int& num)const;

    const std::vector<DERIVATION_DATA>& GetDerivationData()const
    {
        return m_DerivationData;
    }


    int GetTotalDerivChance()const;
    int GetDerivChance(const int& num)const;
    // ƒ‹[ƒgƒm[ƒh—p
    void SetTaskName(const std::string& name)
    {
        m_TaskName = name;
    }
    // Node‚Å‚Ì‚İg—p
    void SetCurrentState(const NODE_STATE& state)
    {
        m_CurrentState = state;
    }
};