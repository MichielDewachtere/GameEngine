#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include <glm/vec3.hpp>

#include "Component.h"

namespace real
{
	class TransformComponent;
	class Scene;
	class Texture2D;
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		GameObject(Scene* pScene)  : m_pScene(pScene) {}
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

		GameObject* CreateGameObject();

		void Init();

		void Update();
		void Render() const;

		template <class T>
		std::shared_ptr<T> AddComponent();
		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T>
		void RemoveComponent();
		template <class T>
		bool HasComponent() const;

		void SetParent(GameObject* pParent, const bool keepWorldPosition);
		GameObject* GetParent() const { return m_pParent; }

		size_t GetChildCount() const { return m_ChildrenPtrs.size(); }
		GameObject* GetChildAt(const unsigned int idx) const { return m_ChildrenPtrs[idx].get(); }
		std::vector<GameObject*> GetChildren() const;

	private:
		Scene* m_pScene{ nullptr };

		std::vector<std::shared_ptr<Component>> m_ComponentPtrs{};

		TransformComponent* m_pTransform{ nullptr };

		//TODO: to solve memory leak make child or parent weakptr otherwise
		//sharedptrs will point to each other and keep each other alive

		GameObject* m_pParent{ nullptr };
		// Could be unique if not shared to scene.
		std::vector<std::unique_ptr<GameObject>> m_ChildrenPtrs{};
		//TODO: what happens to children's children
	};

	template <class T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		if (HasComponent<T>())
			throw std::runtime_error("There is already a component of this type connected to this GameObject");

		const std::shared_ptr<T>pComponent = std::make_shared<T>(this);
		
		m_ComponentPtrs.emplace_back(pComponent);

		return pComponent;
	}

	template <class T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		if (!HasComponent<T>())
			throw std::runtime_error("This GameObject doesn't have a component of this type");

		for (const auto& pComponent : m_ComponentPtrs)
		{
			if (std::shared_ptr<T> otherComponent = std::dynamic_pointer_cast<T>(pComponent))
				return otherComponent;
		}
		return nullptr;
	}

	template <class T>
	void GameObject::RemoveComponent()
	{
		if (HasComponent<T>() == false)
			throw std::runtime_error("This GameObject doesn't have a component of this type");

		for (auto it = m_ComponentPtrs.begin(); it != m_ComponentPtrs.end(); ++it)
		{
			if (std::shared_ptr<T> otherComponent = std::dynamic_pointer_cast<T>(*it))
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
			if (std::dynamic_pointer_cast<T>(pComponent))
				return true;
		}

		return false;
	}
}
