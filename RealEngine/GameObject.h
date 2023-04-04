#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"

#include "Component.h"
#include "Observer.h"

namespace real
{
	class TransformComponent;
	class Scene;
	class Texture2D;
	class Component;
	class Observer;

	class GameObject
	{
	public:
		explicit GameObject() = default;
		explicit GameObject(Scene* pScene) : m_pScene(pScene) {}
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

		GameObject* CreateGameObject();

		template <class T>
		T* CreateGameObject();

		virtual void Init();

		void Update();
		void Render() const;

		//Component Logic
		template <class T>
		T* AddComponent();
		template <class T>
		T* GetComponent();
		template <class T>
		void RemoveComponent();
		template <class T>
		bool HasComponent() const;

		//Child-Parent Logic
		void SetParent(GameObject* pParent, const bool keepWorldPosition);
		GameObject* GetParent() const { return m_pParent; }

		size_t GetChildCount() const { return m_ChildrenPtrs.size(); }
		GameObject* GetChildAt(const unsigned int idx) const { return m_ChildrenPtrs[idx].get(); }
		std::vector<GameObject*> GetChildren() const;

		//Observer Logic
		template <class T>
		void AddObserver();
		template <class T>
		void RemoveObserver();
		template <class T>
		bool HasObserver() const;
		void NotifyObservers(Observer::GameEvent event);

	protected:
		Scene* m_pScene{ nullptr };
		TransformComponent* m_pTransform{ nullptr };

		std::vector<std::unique_ptr<Component>> m_ComponentPtrs{};

		GameObject* m_pParent{ nullptr };
		std::vector<std::unique_ptr<GameObject>> m_ChildrenPtrs{};

		std::vector<std::unique_ptr<Observer>> m_ObserverPtrs{};
	};

	/**
	 * \brief 
	 * \tparam T must be derived from GameObject
	 * \return 
	 */
	template <class T>
	T* GameObject::CreateGameObject()
	{
		if (std::is_base_of<GameObject, T>() == false)
			throw std::runtime_error("Error: T must be derived from GameObject.");

		auto pGameObject{ std::make_unique<T>(m_pScene) };
		pGameObject->Init();
	
		const auto pGameObjectPtr{ pGameObject.get() };
	
		pGameObject->m_pParent = this;
		m_ChildrenPtrs.push_back(std::move(pGameObject));
	
		return pGameObjectPtr;
	}

#pragma region ComponentLogic
	template <class T>
	T* GameObject::AddComponent()
	{
		if (HasComponent<T>())
			throw std::runtime_error("Error: Could not add component to GameObject - a component of this type is already attached. Please remove the existing component before adding another.");

		std::unique_ptr<T> pComponent = std::make_unique<T>(this);

		T* rawPtr = pComponent.get();
		m_ComponentPtrs.emplace_back(std::move(pComponent));

		return rawPtr;
	}
	template <class T>
	T* GameObject::GetComponent()
	{
		if (!HasComponent<T>())
			throw std::runtime_error("Error: Could not find component on GameObject - no component of this type is attached. Please ensure the GameObject has a component of the correct type attached.");

		for (const auto& pComponent : m_ComponentPtrs)
		{
			T* otherComponent = dynamic_cast<T*>(pComponent.get());
			if (otherComponent != nullptr)
				return otherComponent;
		}
		return nullptr;
	}

	template <class T>
	void GameObject::RemoveComponent()
	{
		if (HasComponent<T>() == false)
			throw std::runtime_error("Error: Could not find component on GameObject - no component of this type is attached. Please ensure the GameObject has a component of the correct type attached.");

		for (auto it = m_ComponentPtrs.begin(); it != m_ComponentPtrs.end(); ++it)
		{
			T* otherComponent = dynamic_cast<T*>(*it->get());
			if (otherComponent != nullptr)
			{
				it = m_ComponentPtrs.erase(it);
				break;
			}
		}
	}

	template <class T>
	bool GameObject::HasComponent() const
	{
		for (const auto& pComponent : m_ComponentPtrs)
		{
			if (dynamic_cast<T*>(pComponent.get()))
				return true;
		}

		return false;
	}
#pragma endregion
#pragma region ObserverLogic
	template <class T>
	void GameObject::AddObserver()
	{
		if (HasObserver<T>())
			throw std::runtime_error("Error: Could not add observer to GameObject - a observer of this type is already attached. Please remove the existing observer before adding another.");

		m_ObserverPtrs.emplace_back(std::make_unique<T>());
	}

	template <class T>
	void GameObject::RemoveObserver()
	{
		if (HasObserver<T>() == false)
			throw std::runtime_error("Error: Could not find observer on GameObject - no observer of this type is attached. Please ensure the GameObject has a observer of the correct type attached.");

		for (auto it = m_ObserverPtrs.begin(); it != m_ObserverPtrs.end(); ++it)
		{
			if (dynamic_cast<T*>((*it).get()))
			{
				it = m_ObserverPtrs.erase(it);
				break;
			}
		}
	}

	template <class T>
	bool GameObject::HasObserver() const
	{
		for (const auto& pObserver : m_ObserverPtrs)
		{
			if (dynamic_cast<T*>(pObserver.get()))
				return true;
		}

		return false;
	}
#pragma endregion
}

#endif // GAMEOBJECT_H