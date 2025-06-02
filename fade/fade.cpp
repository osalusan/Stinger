#include "fade.h"
#include "main/main.h"
#include "manager/textureManager.h"
#include "polygon2D/polygon2D.h"

// フェードの最大時間
constexpr float m_FADE_MAX = 1.0f;

Fade::~Fade()
{
	delete m_FadeTexture;
	m_FadeTexture = nullptr;
}

void Fade::Init()
{
	m_IsFadeIn = false;
	m_IsFadeOut = false;
	m_CurrentTime = 0.0f;

	if (m_FadeTexture == nullptr)
	{
		m_FadeTexture = new Polygon2D(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIVOT::CENTER, TEXTURE::WHITE, L"asset\\texture\\white.png");
	}
	if (m_FadeTexture != nullptr)
	{
		m_FadeTexture->Init();
	}
}

void Fade::Uninit()
{
	if (m_FadeTexture != nullptr)
	{
		m_FadeTexture->Uninit();
	}
}

void Fade::Update(const float& deltaTime)
{
	if (!m_IsFadeIn && !m_IsFadeOut) return;

	if (m_IsFadeOut)
	{
		m_CurrentTime += deltaTime;
	}
	else if (m_IsFadeIn)
	{
		m_CurrentTime -= deltaTime;
	}
}

void Fade::Draw()
{
	if (m_FadeTexture == nullptr) return;

	m_FadeTexture->SetAlfa(m_CurrentTime);
	m_FadeTexture->Draw();
}

void Fade::StartFadeIn()
{
	if (m_IsFadeIn || m_IsFadeOut) return;
	m_IsFadeIn = true;
	m_CurrentTime = 1.0f;
}

void Fade::StartFadeOut()
{
	if (m_IsFadeIn || m_IsFadeOut) return;
	m_IsFadeOut = true;
	m_CurrentTime = 0.0f;
}

const bool Fade::GetFadeOutComplete()
{
	if (!m_IsFadeIn && !m_IsFadeOut) return false;

	if (m_CurrentTime < m_FADE_MAX || !m_IsFadeOut)
	{
		return false;
	}

	m_IsFadeOut = false;
	return true;
}

const bool Fade::GetFadeInComplete()
{
	if (!m_IsFadeIn && !m_IsFadeOut) return false;

	if (m_CurrentTime > 0.0f || !m_IsFadeIn)
	{
		return false;
	}

	m_IsFadeIn = false;
	return true;
}
