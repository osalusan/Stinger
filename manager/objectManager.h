#pragma once
#include <list>

enum class OBJECT
{

	MAX
};

// ‘O•ûéŒ¾
class GameObject;

class ObjectManager final
{
private:
	std::list<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)];
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};