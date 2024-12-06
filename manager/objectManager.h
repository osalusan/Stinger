#pragma once
#include <list>
#include <vector>

// �󔒂̔z�񂪐��܂�Ă��܂�����NONE�͒ǉ����Ȃ�
enum class OBJECT
{
	SKYDOME = 0,
	POLYGON2D,
	STATICMESH,
	MAX
};

// �O���錾
class GameObject;
class Player;
class BossEnemy;

class ObjectManager final
{
private:
	std::list<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)] = {};
	Player* m_Player = nullptr;
	BossEnemy* m_Boss = nullptr;
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

	// ���W�w��Œǉ�
	template <typename T, typename... Arg>
	void CreateBossEnemy(Arg&&...args)
	{
		T* boss = new T(std::forward<Arg>(args)...);
		if (boss == nullptr) return;

		boss->Init();
		m_Boss = boss;
	}

	// ���������Œǉ�
	template <typename T>
	void AddGameObject(const OBJECT& layer)
	{
		T* gameObject = new T;
		if (gameObject == nullptr) return;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);
	}

	// �����t���̏ꍇ�͂�����
	template <typename T, typename... Arg>
	void AddGameObjectArg(const OBJECT& layer, Arg&&...args)
	{
		T* gameObject = new T(std::forward<Arg>(args)...);
		if (gameObject == nullptr) return;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);
	}
};