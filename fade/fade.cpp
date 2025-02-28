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
	m_FadeIn = false;
	m_FadeOut = false;
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
	if (!m_FadeIn && !m_FadeOut) return;

	if (m_FadeOut)
	{
		m_CurrentTime += deltaTime;
	}
	else if (m_FadeIn)
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
	if (m_FadeIn || m_FadeOut) return;
	m_FadeIn = true;
	m_CurrentTime = 1.0f;
}

void Fade::StartFadeOut()
{
	if (m_FadeIn || m_FadeOut) return;
	m_FadeOut = true;
	m_CurrentTime = 0.0f;
}

const bool Fade::GetFadeOutComplete()
{
	if (!m_FadeIn && !m_FadeOut) return false;

	if (m_CurrentTime < m_FADE_MAX || !m_FadeOut)
	{
		return false;
	}

	m_FadeOut = false;
	return true;
}

const bool Fade::GetFadeInComplete()
{
	if (!m_FadeIn && !m_FadeOut) return false;

	if (m_CurrentTime > 0.0f || !m_FadeIn)
	{
		return false;
	}

	m_FadeIn = false;
	return true;
}
