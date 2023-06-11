//#include "stdafx.h"
#include "ColliderComponent.h"

#include "Logger.h"
#include "Renderer.h"
#include "TransformComponent.h"

real::ColliderComponent::ColliderComponent(GameObject* pOwner, const glm::vec2& size)
	: Component(pOwner)
{
	if (GetOwner() != nullptr)
		m_Pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
	else
		m_Pos = { 0,0 };

	SetSize(size);
}

real::ColliderComponent::ColliderComponent(GameObject* pOwner, float width, float height)
	: ColliderComponent(pOwner, { width, height })
{
}

void real::ColliderComponent::Update()
{
	m_Pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
}

void real::ColliderComponent::DebugRender() const
{
	if (m_DrawDebug == false)
		return;

	const auto renderer = Renderer::GetInstance().GetSDLRenderer();

	SDL_Rect rect;
	rect.x = static_cast<int>(m_Pos.x + m_Offset.x);
	rect.y = static_cast<int>(m_Pos.y + m_Offset.y);
	rect.w = static_cast<int>(m_Size.x);
	rect.h = static_cast<int>(m_Size.y);

	SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(m_Color.r), static_cast<Uint8>(m_Color.g), static_cast<Uint8>(m_Color.b), static_cast<Uint8>(m_Color.a));
	SDL_RenderDrawRect(renderer, &rect);
}

void real::ColliderComponent::SetSize(const glm::vec2& size)
{
	if (size.y < 0)
	{
		Logger::LogError("In ColliderComponent, the size of a collider must be positive => size.y was {}", size.y);
		return;
	}

	if (size.x < 0)
	{
		Logger::LogError("In ColliderComponent, the size of a collider must be positive => size.x was {}", size.x);
		return;
	}

	m_Size = size;
}

bool real::ColliderComponent::IsOverlapping(const ColliderComponent& other) const
{
	const auto otherSize = other.GetSize();
	const auto otherPos = other.GetPosition();

	const auto pos = m_Pos + m_Offset;

	if (pos.x > otherPos.x + otherSize.x)
		return false;

	if (pos.x + m_Size.x < otherPos.x)
		return false;

	// Y(0,0) IS ON THE TOP RIGHT OF THE SCREEN
	if (pos.y > otherPos.y + otherSize.y)
		return false;

	if (pos.y + m_Size.y < otherPos.y)
		return false;

	return true;
}

bool real::ColliderComponent::IsEntireColliderOverlapping(const ColliderComponent& other, const glm::vec2& offset) const
{
	const auto otherSize = other.GetSize();
	const auto otherPos = other.GetPosition();

	auto pos = m_Pos + m_Offset /*+ offset*/;
	pos.x -= offset.x / 2;
	pos.y -= offset.y / 2;

	auto size = m_Size;
	size.x += offset.x;
	size.y += offset.y;

	if (static_cast<int>(pos.x) > static_cast<int>(otherPos.x))
		return false;

	if (static_cast<int>(pos.x + size.x) < static_cast<int>(otherPos.x + otherSize.x))
		return false;

	// Y(0,0) IS ON THE TOP RIGHT OF THE SCREEN
	if (static_cast<int>(pos.y) > static_cast<int>(otherPos.y))
		return false;

	if (static_cast<int>(pos.y + size.y) < static_cast<int>(otherPos.y + otherSize.y))
		return false;

	return true;
}

bool real::ColliderComponent::IsOverlappingWith(const ColliderComponent& other, const std::string& expectedTag) const
{
	if (other.GetOwner()->GetTag() != expectedTag)
		return false;

	return IsOverlapping(other);
}
