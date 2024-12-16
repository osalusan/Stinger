#pragma once
#include <vector>

enum class NODE_STATUS 
{
    SUCCESS = 0,
    FAILURE,
    RUNNING
};

class BehaviorNode 
{
public:
    virtual ~BehaviorNode() {};
    virtual void Init() = 0;
    virtual NODE_STATUS Update(const float& deltaTime) = 0;
};