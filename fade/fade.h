#pragma once
class Polygon2D;
// 基本的なフェード / 特殊なフェードも作る場合は基底クラスを増やす
class Fade final
{
private:
	Polygon2D* m_FadeTexture = nullptr;

	bool m_IsFadeIn = false;		// フェードイン中
	bool m_IsFadeOut = false;		// フェードアウト中
	float m_CurrentTime = 0.0f;		// フェードの経過時間

public:
	~Fade();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void StartFadeIn();					// フェードインを始めたい時に呼ぶ
	void StartFadeOut();				// フェードの管理を行っている所で、GetFadeInComplete()がtrueになった時に、これを呼ぶように

	const bool GetFadeOutComplete();	// 経過時間からフェードアウト完了の判定を行う
	const bool GetFadeInComplete();		// 経過時間からフェードイン完了の判定を行う
}; 
