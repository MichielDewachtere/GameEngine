#include "SpriteComponent.h"

#include <glm/vec2.hpp>

#include "GameTime.h"
#include "SDLRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"


real::SpriteComponent::SpriteComponent(GameObject* pOwner, SpriteSheet spriteSheet)
	: Component(pOwner)
	, m_SpriteSheet(std::move(spriteSheet))
{
	m_Rect.w = m_SpriteSheet.pTexture->GetSize().x / m_SpriteSheet.columns;
	m_Rect.h = m_SpriteSheet.pTexture->GetSize().y / m_SpriteSheet.rows;
}

void real::SpriteComponent::Update()
{
	m_RenderPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();

	if (m_Pause || m_Stop)
	{
		m_IsAnimationPlaying = false;
		return;
	}

	m_AccuTime += Time::GetInstance().GetElapsed();

	if (m_AccuTime > m_SpriteSheet.animTimer)
	{
		//UpdateCurrIdx
		++m_CurrIdx;
		if (m_PlaySelectedIdcs == false)
		{
			if (m_CurrIdx >= m_EndIdx)
			{
				++m_AccuLoops;

				if (m_Loops != -1 && m_AccuLoops > m_Loops)
				{
					m_Stop = true;
				}

				m_CurrIdx = m_StartIdx;
			}

			//Move rect
			m_Rect.x = (m_CurrIdx % m_SpriteSheet.columns) * m_Rect.w;
			m_Rect.y = (m_CurrIdx / m_SpriteSheet.columns) * m_Rect.h;
		}
		else
		{
			if (m_CurrIdx >= static_cast<int>(m_SelectedIdcs.size()))
			{
				++m_AccuLoops;

				if (m_Loops != -1 && m_AccuLoops > m_Loops)
				{
					m_Stop = true;
				}

				m_CurrIdx = 0;
			}

			m_Rect.x = (m_SelectedIdcs[m_CurrIdx] % m_SpriteSheet.columns) * m_Rect.w;
			m_Rect.y = (m_SelectedIdcs[m_CurrIdx] / m_SpriteSheet.columns) * m_Rect.h;
		}

		m_AccuTime = 0.f;
	}
}

void real::SpriteComponent::Render() const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(m_RenderPos.x);
	dst.y = static_cast<int>(m_RenderPos.y);
	dst.w = static_cast<int>(m_Rect.w);
	dst.h = static_cast<int>(m_Rect.h);

	const SDL_Point rotationCenter{ dst.w / 2, dst.h / 2 };

	SDL_RenderCopyEx(SDLRenderer::GetInstance().GetSDLRenderer(), m_SpriteSheet.pTexture->GetSDLTexture(), &m_Rect, &dst, 0, &rotationCenter, m_Flip);
}

void real::SpriteComponent::SelectSprite(int idx)
{
	m_Pause = true;

	m_Rect.x = (idx % m_SpriteSheet.columns) * m_Rect.w;
	m_Rect.y = (idx / m_SpriteSheet.columns) * m_Rect.h;
}

void real::SpriteComponent::PlayAnimation(int startIdx, int endIdx, int loops)
{
	m_AccuTime = 0.f;

	m_StartIdx = startIdx;
	m_EndIdx = endIdx;
	m_CurrIdx = m_StartIdx;

	m_Loops = loops;

	m_Pause = false;
	m_Stop = false;
	m_IsAnimationPlaying = true;

	m_Flip = SDL_FLIP_NONE;
}

void real::SpriteComponent::PlayAnimation(const std::vector<int>& idcs, int loops)
{
	m_PlaySelectedIdcs = true;
	m_SelectedIdcs = idcs;

	m_Loops = loops;

	m_CurrIdx = 0;

	m_Pause = false;
	m_Stop = false;
	m_IsAnimationPlaying = true;

	m_Flip = SDL_FLIP_NONE;
}

void real::SpriteComponent::UpdateAnimation(int startIdx, int endIdx, int loops)
{
	m_AccuTime = 0.f;

	m_StartIdx = startIdx;
	m_EndIdx = endIdx;
	m_CurrIdx = m_StartIdx;

	m_Loops = loops;
}

void real::SpriteComponent::Pause(bool value)
{
	m_Pause = value;
}

void real::SpriteComponent::Stop(bool value)
{
	m_Stop = value;

	if (m_Stop)
		m_IsAnimationPlaying = false;
}

void real::SpriteComponent::FlipTexture(SDL_RendererFlip flip)
{
	m_Flip = flip;
}

glm::vec2 real::SpriteComponent::GetSpriteSize() const
{
	return glm::vec2(m_Rect.w, m_Rect.h);
}