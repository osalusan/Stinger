#pragma once
#include <list>
#include <vector>

// �󔒂̔z�񂪐��܂�Ă��܂�����NONE�͒ǉ����Ȃ�
enum class OBJECT
{
	SKYDOME = 0,
	STATICMESH,
	SHADOW,
	POLYGON2D,
	MAX
};

// �O���錾
class GameObject;
class Camera;
class Player;
class BossEnemy;
class MeshFiled;

class ObjectManager final
{
private:
	std::list<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)] = {};
	Camera* m_Camera = nullptr;
	Player* m_Player = nullptr;
	BossEnemy* m_Boss = nullptr;
	MeshFiled* m_Filed = nullptr;
public:
	~ObjectManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void CreatePlayer();

	Camera* GetCamera()const
	{
		return m_Camera;
	}
	Player* GetPlayer()const
	{
		return m_Player;
	}
	BossEnemy* GetBossEnemy()const
	{
		return m_Boss;
	}
	MeshFiled* GetMeshFiled()const
	{
		return m_Filed;
	}
	void GetAllGameObjects(std::vector<GameObject*> (&objectList)[static_cast<int>(OBJECT::MAX)])
	{
		for (int i = 0; i < static_cast<int>(OBJECT::MAX); i++)
		{
			objectList[i].insert(objectList[i].end(), m_GameObjects[i].begin(), m_GameObjects[i].end());
		}
	}

	void GetGameObjects(std::vector<GameObject*>(&objectList),const OBJECT& layer)
	{
		const int& layerNum = static_cast<int>(layer);
		objectList.insert(objectList.end(), m_GameObjects[layerNum].begin(), m_GameObjects[layerNum].end());
	}
	// �J�����I�u�W�F�N�g����ԍŏ��ɍ쐬
	template <typename T>
	void CreateCameraObject()
	{
		if (m_Camera != nullptr) return;
		T* camera = new T;
		if (camera == nullptr) return;

		camera->Init();

		m_Camera = camera;
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

	template <typename T>
	void CreateMeshFiled()
	{
		if (m_Filed != nullptr) return;
		T* filed = new T;
		if (filed == nullptr) return;

		filed->Init();
		m_Filed = filed;
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