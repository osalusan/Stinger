#pragma once
#include "staticmeshObject.h"

class Ruins final: public StaticMeshObject
{
public:
	Ruins() = delete;
	Ruins(const XMFLOAT3& position, const XMFLOAT3& scale, const XMFLOAT3& rotate);
};