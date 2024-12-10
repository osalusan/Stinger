#pragma once

class ObjectManager;
class ParticleManager;
class Camera;

class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;
	ParticleManager* m_ParticleManager = nullptr;

	Camera* m_Camera = nullptr;

	void CreateParticleManager();
public:
	virtual ~Scene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	template <typename T>
	void AddCameraObject()
	{
		if (m_Camera != nullptr) return;
		T* camera = new T;
		if (camera == nullptr) return;

		camera->Init();

		m_Camera = camera;
	}

	Camera* GetCamera()
	{
		return m_Camera;
	}

	ObjectManager* GetObjectManager()
	{
		return m_ObjectManager;
	}
	

};