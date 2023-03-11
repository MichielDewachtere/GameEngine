#include "GameObject.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "SceneManager.h"

real::GameObject::~GameObject() = default;

real::GameObject* real::GameObject::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(m_pScene) };
	pGameObject->Init();

	const auto pGameObjectPtr{ pGameObject.get() };

	//pGameObject->SetParent(this, true);
	pGameObject->m_pParent = this;
	m_ChildrenPtrs.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

void real::GameObject::Init()
{
	m_pTransform = this->AddComponent<TransformComponent>().get();
}

void real::GameObject::Update()
{
	for (const auto& pComponent : m_ComponentPtrs)
		pComponent->Update();

	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Update();
}

void real::GameObject::Render() const
{
	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->CanRender()) 
			pComponent->Render();
	}
	
	for (const auto& pChild : m_ChildrenPtrs)
		pChild->Render();
}

void real::GameObject::SetParent(GameObject* pParent, const bool keepWorldPosition [[maybe_unused]] )
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