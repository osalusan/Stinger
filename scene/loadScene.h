#pragma once
#include "scene/scene.h"

class LoadScene final :public Scene
{
private:
	bool m_Load = false;

public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;

	void LoadStart()
	{
		m_Load = true;
	}

	const bool& GetLoad()const
	{
		return m_Load;
	}
};