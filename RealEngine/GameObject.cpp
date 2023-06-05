//#include "stdafx.h"
#include "GameObject.h"

#include <algorithm>

#include "Scene.h"
#include "TransformComponent.h"

real::GameObject::~GameObject() = default;

real::GameObject* real::GameObject::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(m_pScene) };
	pGameObject->Init();

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


void real::GameObject::Init()
{
	m_pTransform = AddComponent<TransformComponent>();
}

void real::GameObject::Start()
{
	for (const auto& pComponent : m_ComponentPtrs)
		pComponent->Start();

	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Start();
}

void real::GameObject::Update()
{
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

			// if (m_IsMarkedForDestroy)
			//     pChild->Destroy();

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

	if (m_IsMarkedForDestroy)
	{
		//for (const auto& pComponent : m_ComponentPtrs)
		//{
		//	std::erase(m_ComponentPtrs, pComponent);
		//}

		for (auto& pComponent : m_ComponentPtrs)
		{
			RemoveComponent(*pComponent);
		}

		m_ComponentPtrs.clear();
	}
}

void real::GameObject::Destroy()
{
	m_IsMarkedForDestroy = true;

	for (const auto& pChild : m_ChildrenPtrs)
	{
		pChild->Destroy();
	}

	//m_ComponentPtrs.clear();

	//for (const auto& pChild : m_ChildrenPtrs)
	//{
	//	pChild->Destroy();
	//}

	//m_pParent->RemoveChild(std::unique_ptr<GameObject>(this));
}

void real::GameObject::SetParent(GameObject* pParent, const bool keepWorldPosition)
{
	if (pParent == nullptr)
		m_pTransform->SetLocalPosition(m_pTransform->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() - pParent->m_pTransform->GetWorldPosition());
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

		//const std::unique_ptr<GameObject> pThis(this);
		//pParent->m_ChildrenPtrs.push_back(pThis);
	}
}

std::vector<real::GameObject*> real::GameObject::GetChildren() const
{
	std::vector<GameObject*> childrenPtrs;
	childrenPtrs.reserve(m_ChildrenPtrs.size());

	for (const auto& pChild : m_ChildrenPtrs)
	{
		childrenPtrs.push_back(pChild.get());
	}

	return childrenPtrs;
}

void real::GameObject::RemoveChild(std::unique_ptr<GameObject> gameObject)
{
	//for (const auto& pChild: gameObject->GetChildren())
	//{
	//	gameObject->RemoveChild(std::unique_ptr<GameObject>(pChild));
	//}
	std::erase(m_ChildrenPtrs, gameObject);
}
