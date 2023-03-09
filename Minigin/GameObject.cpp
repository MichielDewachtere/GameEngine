#include "GameObject.h"

#include "ResourceManager.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	//if it owns the children, it updates the children <=> scene
	for (const auto& pComponent : m_ComponentPtrs)
	{
		pComponent->Update();
	}
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->CanRender()) 
			pComponent->Render();
	}
}

void dae::GameObject::SetParent(const std::shared_ptr<GameObject>& parent, const bool keepWorldPosition)
{
	if (parent == nullptr)
		SetLocalPosition(m_WorldPosition);
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(m_LocalPosition - parent->GetWorldPosition());
		m_PositionIsDirty = true;	
	}

	if (m_pParent)
		m_pParent->SetParent(nullptr, true);

	m_pParent = parent;
	//if (m_pParent)
		//m_pParent->AddChild(weak_from_this());
}

bool dae::GameObject::RemoveChild(const std::shared_ptr<GameObject>& go)
{
	if (go == nullptr)
		return false;

	if (m_ChildrenPtrs.empty())
		return false;

	//Remove the given child from the children list
	if (std::remove(m_ChildrenPtrs.begin(), m_ChildrenPtrs.end(), go) != m_ChildrenPtrs.end())
	{
		//Remove itself as a parent of the child
		//Update position, rotation and scale
		go->SetParent(nullptr, true);

		return true;
	}

	//TODO: 

	return false;
}

void dae::GameObject::AddChild(const std::weak_ptr<dae::GameObject>& go)
{
	if (go.expired())
		return;

	////Remove the given child from the child's previous parent
	//go->SetParent(nullptr, true);

	////Set itself as parent of the child
	////Update position, rotation and scale
	//go->SetParent(shared_from_this(), false);

	//Add the child to its children list
	m_ChildrenPtrs.push_back(go.lock());
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	//TODO: children are not set dirty
	m_LocalPosition = pos;
	m_PositionIsDirty = true;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionIsDirty = false;
}