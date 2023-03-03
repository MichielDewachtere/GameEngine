#pragma once
#include <memory>
#include <vector>
#include <stdexcept>

#include "Component.h"

namespace dae
{
	class Texture2D;
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

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

	private:
		std::vector<std::shared_ptr<Component>> m_ComponentPtrs{};
	};

	template <class T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		if (HasComponent<T>())
			throw std::runtime_error("There is already a component of this type connected to this GameObject");

		const std::shared_ptr<T>pComponent = std::make_shared<T>(shared_from_this());
		
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
		if (HasComponent<T>())
			return;

		// REMOVE COMPONENT LOGIC
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
