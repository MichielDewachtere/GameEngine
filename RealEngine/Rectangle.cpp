#include "Rectangle.h"

#include "SDLRenderer.h"

real::Rectangle::Rectangle(GameObject* pOwner)
	: Component(pOwner)
{
}

void real::Rectangle::Render() const
{
	SDLRenderer::GetInstance().RenderRectangle(m_Rectangle, m_IsFilled, m_Color);
}

void real::Rectangle::SetRect(SDL_Rect rect)
{
	m_Rectangle = rect;
}

void real::Rectangle::SetRect(int x, int y, int width, int height)
{
	SetRect(SDL_Rect{ x, y, width, height });
}
