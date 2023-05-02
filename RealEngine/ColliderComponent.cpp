#include "stdafx.h"
#include "ColliderComponent.h"

#include "Renderer.h"
#include "TransformComponent.h"

real::ColliderComponent::ColliderComponent(GameObject* pOwner, const glm::vec2& size)
	: Component(pOwner)
	, m_Size(size)
{
	if (GetOwner() != nullptr)
		m_Pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
	else
		m_Pos = { 0,0 };
}

real::ColliderComponent::ColliderComponent(GameObject* pOwner, float width, float height)
	: ColliderComponent(pOwner, { width, height })
{
}

void real::ColliderComponent::Update()
{
	//m_Pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
}

void real::ColliderComponent::DebugRender() const
{
	const auto renderer = Renderer::GetInstance().GetSDLRenderer();

	SDL_Rect rect;
	rect.x = static_cast<int>(m_Pos.x);
	rect.y = static_cast<int>(m_Pos.y);
	rect.w = static_cast<int>(m_Size.x);
	rect.h = static_cast<int>(m_Size.y);

	SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(m_Color.r), static_cast<Uint8>(m_Color.g), static_cast<Uint8>(m_Color.b), static_cast<Uint8>(m_Color.a));
	SDL_RenderDrawRect(renderer, &rect);
}

bool real::ColliderComponent::IsOverlapping(const ColliderComponent& other) const
{
	const auto otherSize = other.GetSize();
	const auto otherPos = other.GetPosition();

	if (m_Pos.x > otherPos.x + otherSize.x)
		return false;

	if (m_Pos.x + m_Size.x < otherPos.x)
		return false;

	// Y(0,0) IS ON THE TOP RIGHT OF THE SCREEN
	if (m_Pos.y > otherPos.y + otherSize.y)
		return false;

	if (m_Pos.y + m_Size.y < otherPos.y)
		return false;

	return true;
}

bool real::ColliderComponent::IsEntireColliderOverlapping(const ColliderComponent& other) const
{
	const auto otherSize = other.GetSize();
	const auto otherPos = other.GetPosition();

	if (static_cast<int>(m_Pos.x) > static_cast<int>(otherPos.x))
		return false;

	if (static_cast<int>(m_Pos.x + m_Size.x) < static_cast<int>(otherPos.x + otherSize.x))
		return false;

	// Y(0,0) IS ON THE TOP RIGHT OF THE SCREEN
	if (static_cast<int>(m_Pos.y) > static_cast<int>(otherPos.y))
		return false;

	if (static_cast<int>(m_Pos.y + m_Size.y) < static_cast<int>(otherPos.y + otherSize.y))
		return false;

	return true;
}

bool real::ColliderComponent::IsOverlappingWith(const ColliderComponent& other, const std::string& expectedTag) const
{
	if (other.GetOwner()->GetTag() != expectedTag)
		return false;

	return IsOverlapping(other);
}
