#pragma once
#include <list>

enum class OBJECT
{
	SKYDOME = 0,
	POLYGON2D,
	MAX
};

// ‘O•ûéŒ¾
class GameObject;
class Player;

class ObjectManager final
{
private:
	std::list<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)];
	Player* m_Player = nullptr;

public:
	~ObjectManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	Player* GetPlayer()
	{
		return m_Player;
	}

	// ˆø”–³‚µ‚Å’Ç‰Á
	template <typename T>
	void AddGameObject(const OBJECT& layer)
	{
		T* Gameobject = new T;
		if (Gameobject == nullptr) return;

		Gameobject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(Gameobject);
	}

	// ˆø”•t‚«‚Ìê‡‚Í‚±‚Á‚¿
	template <typename T, typename... Arg>
	void AddGameObjectArg(const OBJECT& layer, Arg&&...args)
	{
		T* GameObject = new T(std::forward<Arg>(args)...);
		if (GameObject == nullptr) return;

		GameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(GameObject);
	}
};