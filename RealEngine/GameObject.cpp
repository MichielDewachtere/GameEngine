//#include "stdafx.h"
#include "GameObject.h"

#include <algorithm>

#include "Scene.h"
#include "TransformComponent.h"

real::GameObject* real::GameObject::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(m_pScene) };
	pGameObject->AddComponent<TransformComponent>();

	const auto pGameObjectPtr{ pGameObject.get() };
	
	pGameObject->m_pParent = this;
	m_ChildrenPtrs.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

std::vector<real::GameObject*> real::GameObject::GetObjectsWithTag(const std::string& tag)
{
	std::vector<GameObject*> goPtrs;

	if (m_Tag == tag)
		goPtrs.push_back(this);

	for (const auto& gameObject : m_ChildrenPtrs)
	{
		for (const auto go : gameObject->GetObjectsWithTag(tag))
		{
			goPtrs.push_back(go);
		}
	}

	return goPtrs;
}

real::GameObject* real::GameObject::GetObject(gameobject_id id)
{
	if (id == m_Id)
		return this;

	for (const auto& go : m_ChildrenPtrs)
	{
		auto pGo = go->GetObject(id);
		if (pGo != nullptr)
			return pGo;
	}

	return nullptr;
}

void real::GameObject::Start()
{
	if (m_IsActive == false)
		return;

	for (const auto& pComponent : m_ComponentPtrs)
		pComponent->Start();

	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Start();
}

void real::GameObject::Update()
{
	if (m_IsActive == false)
		return;

	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->GetIsActive())
			pComponent->Update();
	}

	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Update();
}

void real::GameObject::Render() const
{
	if (m_IsActive == false)
		return;

	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->CanRender() && pComponent->GetIsActive())
			pComponent->Render();
	}
	
	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Render();
}

void real::GameObject::DebugRender() const
{
	if (m_IsActive == false)
		return;

	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->CanRender() && pComponent->GetIsActive())
			pComponent->DebugRender();
	}

	for (const auto& pChild : m_ChildrenPtrs)
		pChild->DebugRender();

}

void real::GameObject::PostUpdate()
{
	if (m_ChildrenPtrs.empty() == false)
	{
		for (auto it = m_ChildrenPtrs.begin(); it != m_ChildrenPtrs.end(); )
		{
			auto& pChild = *it;

			if (m_IsActive)
				pChild->PostUpdate();

			if (pChild->IsMarkedForDestroy() && pChild->GetId() <= m_NextId)
			{
				it = m_ChildrenPtrs.erase(it); // Erase the element and update the iterator
			}
			else
			{
				++it; // Move to the next element
			}
		}
	}

	for (const auto& pComponent : m_ComponentPtrs)
	{
		pComponent->PostUpdate();
	}

	if (m_IsMarkedForDestroy)
	{
		for (auto& pComponent : m_ComponentPtrs)
		{
			RemoveComponent(*pComponent);
		}

		m_ComponentPtrs.clear();
	}
}

void real::GameObject::Destroy()
{
	if (m_CanBeDestroyed == false)
		return;

	m_IsMarkedForDestroy = true;

	for (const auto& pChild : m_ChildrenPtrs)
	{
		pChild->Destroy();
	}
}

void real::GameObject::SetIsActive(const bool value)
{
	m_IsActive = value;

	for (const auto& pChild : m_ChildrenPtrs)
	{
		pChild->SetIsActive(value);
	}
}

void real::GameObject::SetParent(GameObject* pParent, const bool keepWorldPosition)
{
	if (pParent == nullptr)
		GetComponent<TransformComponent>()->SetLocalPosition(GetComponent<TransformComponent>()->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			GetComponent<TransformComponent>()->SetLocalPosition(GetComponent<TransformComponent>()->GetLocalPosition() - pParent->GetComponent<TransformComponent>()->GetWorldPosition());
	}

	if (GameObject* pOldParent = m_pParent)
	{
		if (pOldParent == pParent) 
			return;
		
		for (size_t i{}; i <= pOldParent->GetChildCount(); ++i)
		{
			auto& pChild = pOldParent->m_ChildrenPtrs[i];

			if (pChild.get() == this)
			{
				//Add Child
				if (pParent) 
					pParent->m_ChildrenPtrs.push_back(std::move(pChild));
				else
					m_pScene->Add(std::move(pChild));

				//Remove Child
				std::swap(pOldParent->m_ChildrenPtrs[i], pOldParent->m_ChildrenPtrs.back());
				pOldParent->m_ChildrenPtrs.pop_back();

				m_pParent = pParent;

				break;
			}
		}
	}
	else
	{
		if (!pParent) 
			return;
	}
}

std::vector<real::GameObject*> real::GameObject::GetChildren() const
{
	std::vector<GameObject*> childrenPtrs{};

	if (m_ChildrenPtrs.empty())
		return childrenPtrs;

	childrenPtrs.reserve(m_ChildrenPtrs.size());

	for (const auto& pChild : m_ChildrenPtrs)
	{
		childrenPtrs.push_back(pChild.get());
	}

	return childrenPtrs;
}

void real::GameObject::RemoveChild(std::unique_ptr<GameObject> gameObject)
{
	gameObject->Destroy();
}
