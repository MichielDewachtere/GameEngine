#include "Rectangle.h"

#include "Locator.h"

real::Rectangle::Rectangle(GameObject* pOwner)
	: Component(pOwner)
{
}

void real::Rectangle::Render() const
{
	Locator::GetRenderSystem().RenderRectangle(m_Rectangle, m_IsFilled, m_Color);
}

void real::Rectangle::SetRect(glm::ivec4 rect)
{
	m_Rectangle = rect;
}

void real::Rectangle::SetRect(int x, int y, int width, int height)
{
	SetRect(glm::ivec4{ x, y, width, height });
}
