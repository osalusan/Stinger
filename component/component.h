#pragma once

class GameObject;
// コンポーネントの抽象クラス
class Component {
protected:
	GameObject* m_GameObject = nullptr;

public:
	Component() = delete;
	Component(GameObject* object) 
	{
		m_GameObject = object;
	}
	virtual ~Component() 
	{ 
		m_GameObject = nullptr; 
	}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
