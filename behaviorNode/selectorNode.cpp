#include "selectorNode.h"

void SelectorNode::Init()
{
    // ƒˆ‰¼‘zŠÖ”‚Ìˆ×‹óŽÀ‘•
}

NODE_STATE SelectorNode::Update(const float& deltaTime)
{
    for (BehaviorNode* child : m_Children)
    {
        if (child == nullptr) continue;

        NODE_STATE state = child->Update(deltaTime);
        child->SetCurrentState(state);
        m_CurrentState = state;

        if (state != NODE_STATE::FAILURE)
        {       
            return state;
        }
    }
    m_CurrentState = NODE_STATE::FAILURE;
    return NODE_STATE::FAILURE;
}
