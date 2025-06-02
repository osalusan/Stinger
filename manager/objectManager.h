#pragma once
#include <vector>
#include "character/player.h"
#include "character/bossEnemy.h"
#include "camera/camera.h"
#include "meshFiled/meshFiled.h"

// 空白の配列が生まれてしまうからNONEは追加しない
// 更新順、描画順になっている
enum class OBJECT
{
	CAMERA_MAIN = 0,
	SKYDOME,
	FILED,
	PLAYER,
	BOSS,
	ATTACK,
	STATICMESH,
	SHADOW,
	PARTICLE,
	BILLBOARD,
	POLYGON2D,
	MAX
};

class BossEnemy;
class MeshFiled;
// ゲーム内すべてのGameObjectを管理するクラス
class ObjectManager final
{
private:
	std::vector<GameObject*> m_GameObjects[static_cast<int>(OBJECT::MAX)] = {};
	Camera* m_CameraCache = nullptr;
	Player* m_PlayerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;
	MeshFiled* m_FiledCache = nullptr;

	float m_SlowTime = 0.0f;
	float m_SlowValue = 0.0f;
	bool m_SlowEnemy = false;
public:
	~ObjectManager();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	Camera* GetCamera()
	{
		return m_CameraCache;
	}
	Player* GetPlayer()
	{
		return m_PlayerCache;
	}
	BossEnemy* GetBossEnemy()
	{
		return m_BossCache;
	}
	MeshFiled* GetMeshFiled()
	{
		return m_FiledCache;
	}
	bool GetSlow()
	{
		if (m_SlowTime <= 0.0f) return false;
		return true;
	}
	void SetSlowTime(const float& time);
	void SetSlowTimeEnemy(const float& time);
	void SetSlowValue(const float& value)
	{
		m_SlowValue = value;
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

	// 引数無しで追加
	template <typename T>
	T* AddGameObject(const OBJECT& layer)
	{
		T* gameObject = new T;
		if (gameObject == nullptr) return nullptr;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);

		if (layer == OBJECT::CAMERA_MAIN && m_CameraCache == nullptr)
		{
			if (Camera* camera = dynamic_cast<Camera*>(gameObject))
			{
				m_CameraCache = camera;
			}
		}

		return gameObject;
	}

	// 引数付きの場合はこっち
	template <typename T, typename... Arg>
	T* AddGameObjectArg(const OBJECT& layer, Arg&&...args)
	{
		T* gameObject = new T(std::forward<Arg>(args)...);
		if (gameObject == nullptr) return nullptr;

		gameObject->Init();
		m_GameObjects[static_cast<int>(layer)].emplace_back(gameObject);

		if (layer == OBJECT::PLAYER && m_PlayerCache == nullptr)
		{
			if (Player* player = dynamic_cast<Player*>(gameObject))
			{
				m_PlayerCache = player;
			}
		}
		if (layer == OBJECT::BOSS && m_BossCache == nullptr)
		{
			if (BossEnemy* boss = dynamic_cast<BossEnemy*>(gameObject))
			{
				m_BossCache = boss;
			}
		}
		if (layer == OBJECT::FILED && m_FiledCache == nullptr)
		{
			if (MeshFiled* filed = dynamic_cast<MeshFiled*>(gameObject))
			{
				m_FiledCache = filed;
			}
		}
		return gameObject;
	}
};