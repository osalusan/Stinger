#pragma once

class ObjectManager;
class Camera;

class Scene 
{
protected:
	ObjectManager* m_ObjectManager = nullptr;
	Camera* m_Camera = nullptr;

public:
	virtual ~Scene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	Camera* GetCamera()
	{
		return m_Camera;
	}

	ObjectManager* GetObjectManager()
	{
		return m_ObjectManager;
	}
};