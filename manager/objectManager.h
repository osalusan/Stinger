#pragma once
#include <list>
#include <vector>


enum class OBJECT
{
	SKYDOME = 0,
	POLYGON2D,
	BOX,
	MAX
};

// ‘O•ûéŒ¾
class GameObject;
class Player;

class ObjectManager final
{
private:
	std::list<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)] = {};
	Player* m_Player = nullptr;

public:
	~ObjectManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void CreatePlayer();

	Player* GetPlayer()
	{
		return m_Player;
	}

	void GetGameObjects(std::vector<GameObject*> (&objectList)[static_cast<int>(OBJECT::MAX)])
	{
		for (int i = 0; i < static_cast<int>(OBJECT::MAX); i++)
		{
			objectList[i].insert(objectList[i].end(), m_GameObjects[i].begin(), m_GameObjects[i].end());
		}
	}

	// ˆø”–³‚µ‚Å’Ç‰Á
	template <typename T>
	void AddGameObject(const OBJECT& layer)
	{
		T* gameObject = new T;
		if (gameObject == nullptr) return;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);
	}

	// ˆø”•t‚«‚Ìê‡‚Í‚±‚Á‚¿
	template <typename T, typename... Arg>
	void AddGameObjectArg(const OBJECT& layer, Arg&&...args)
	{
		T* gameObject = new T(std::forward<Arg>(args)...);
		if (gameObject == nullptr) return;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);
	}
};