//#include "stdafx.h"
#include "ColliderComponent.h"

#include "Input.h"
#include "Locator.h"
#include "Logger.h"
#include "SDLRenderer.h"
#include "TransformComponent.h"

real::ColliderComponent::ColliderComponent(GameObject* pOwner, const glm::vec2& size)
	: Component(pOwner)
{
	if (pOwner != nullptr)
		GetOwner()->GetComponent<TransformComponent>()->changedWorldPosition.AddObserver(this);

	SetSize(size);
}

real::ColliderComponent::ColliderComponent(GameObject* pOwner, float width, float height)
	: ColliderComponent(pOwner, { width, height })
{
}

real::ColliderComponent::~ColliderComponent()
{
	//if (GetOwner() != nullptr)+
	//	GetOwner()->GetComponent<TransformComponent>()->changedWorldPosition.RemoveObserver(this);
}

void real::ColliderComponent::Start()
{
	if (GetOwner() != nullptr)
		m_Pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
	else
		m_Pos = { 0,0 };
}

void real::ColliderComponent::Update()
{
	if (m_IsDirty)
	{
		m_Pos += m_Offset;
		//m_Offset = { 0,0 };
		m_IsDirty = false;
	}
}

void real::ColliderComponent::DebugRender() const
{
	if (m_DrawDebug == false)
		return;

	Locator::GetRenderSystem().RenderRectangle({ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y }, false, m_Color);
}

void real::ColliderComponent::HandleEvent(glm::ivec2 newPos)
{
	m_Pos = newPos;
	m_Pos += m_Offset;
}

void real::ColliderComponent::SetSize(const glm::vec2& size)
{
	if (size.y < 0)
	{
		Logger::LogError({"In ColliderComponent, the size of a collider must be positive => size.y was {}"}, size.y);
		return;
	}

	if (size.x < 0)
	{
		Logger::LogError({"In ColliderComponent, the size of a collider must be positive => size.x was {}"}, size.x);
		return;
	}

	m_Size = size;
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

bool real::ColliderComponent::IsEntireColliderOverlapping(const ColliderComponent& other, const glm::vec2& offset) const
{
	const auto otherSize = other.GetSize();
	const auto otherPos = other.GetPosition();

	auto pos = m_Pos;
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

bool real::ColliderComponent::IsMouseOverlapping() const
{
	const auto mousePos = Input::GetInstance().GetMousePosition();

	return mousePos.x > m_Pos.x
		&& mousePos.y > m_Pos.y
		&& mousePos.x < m_Pos.x + m_Size.x
		&& mousePos.y < m_Pos.y + m_Size.y;
}
