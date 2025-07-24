#include "behaviorTree.h"
#include "behaviorNode/behaviorNode.h"
#include "main/main.h"

BehaviorTree::~BehaviorTree()
{
    SAFE_DELETE(m_Root);
}

void BehaviorTree::CreateRoot(BehaviorNode* root)
{
    if (m_Root == nullptr && root != nullptr)
    {
        m_Root = root;
    }
    if (m_Root != nullptr)
    {
        m_Root->Init();
        m_Root->SetTaskName("ルートノード");
    }
}

void BehaviorTree::ClearNodeState(BehaviorNode* root)
{
    if (root == nullptr) return;

    for (BehaviorNode* child : root->GetChildren())
    {
        ClearNodeState(child);
    }
    root->SetCurrentState(NODE_STATE::FAILURE);
}

void BehaviorTree::Update(const float& deltaTime)
{
    if (m_Root == nullptr) return;
    ClearNodeState(m_Root);
    NODE_STATE state = m_Root->Update(deltaTime);
}
