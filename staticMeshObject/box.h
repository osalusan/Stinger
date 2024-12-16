#pragma once
#include "staticmeshObject.h"

class Box : public StaticMeshObject 
{
public:
	Box();
	Box(const XMFLOAT3& position, const XMFLOAT3& scale);
	Box(const XMFLOAT3& position,const XMFLOAT3& scale,const XMFLOAT3& rotate);
};