#pragma once
#include <list>
#include <vector>

// �󔒂̔z�񂪐��܂�Ă��܂�����NONE�͒ǉ����Ȃ�
enum class OBJECT
{
	CAMERA = 0,
	SKYDOME,
	FILED,
	PLAYER,
	BOSS,
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
	Camera* m_CameraCache = nullptr;
	Player* m_PlayerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;
	MeshFiled* m_FiledCache = nullptr;
public:
	~ObjectManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void CreatePlayer();

	Camera* GetCamera()const
	{
		return m_CameraCache;
	}
	Player* GetPlayer()const
	{
		return m_PlayerCache;
	}
	BossEnemy* GetBossEnemy()const
	{
		return m_BossCache;
	}
	MeshFiled* GetMeshFiled()const
	{
		return m_FiledCache;
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
		if (m_CameraCache != nullptr) return;
		T* camera = new T;
		if (camera == nullptr) return;

		camera->Init();

		m_GameObjects[static_cast<int>(OBJECT::CAMERA)].emplace_back(camera);
		m_CameraCache = camera;
	}

	// ���W�w��Œǉ�
	template <typename T, typename... Arg>
	void CreateBossEnemy(Arg&&...args)
	{
		if (m_BossCache != nullptr) return;
		T* boss = new T(std::forward<Arg>(args)...);
		if (boss == nullptr) return;

		boss->Init();

		m_GameObjects[static_cast<int>(OBJECT::BOSS)].emplace_back(boss);
		m_BossCache = boss;
	}

	template <typename T>
	void CreateMeshFiled()
	{
		if (m_FiledCache != nullptr) return;
		T* filed = new T;
		if (filed == nullptr) return;

		filed->Init();

		m_GameObjects[static_cast<int>(OBJECT::FILED)].emplace_back(filed);
		m_FiledCache = filed;
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