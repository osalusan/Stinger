#pragma once
class Polygon2D;
class Fade final
{
private:
	Polygon2D* m_FadeTexture = nullptr;

	bool m_FadeIn = false;
	bool m_FadeOut = false;
	float m_CurrentTime = 0.0f;

public:
	~Fade();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void StartFadeIn();
	void StartFadeOut();

	const bool GetFadeOutComplete();
	const bool GetFadeInComplete();
}; 
