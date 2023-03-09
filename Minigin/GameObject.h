#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include <glm/vec3.hpp>

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

		void SetLocalPosition(const glm::vec3& pos);
		//void SetWorldPosition(const glm::vec3& worldPos) { m_WorldPosition = worldPos; }
		const glm::vec3& GetWorldPosition();

		void SetParent(const std::shared_ptr<GameObject>& parent, const bool keepWorldPosition);
		std::shared_ptr<GameObject> GetParent() const { return m_pParent; }

		size_t GetChildCount() const { return m_ChildrenPtrs.size(); }
		std::shared_ptr<GameObject> GetChildAt(unsigned int idx) const;
		std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_ChildrenPtrs; }
		//TODO: what happens to children's children
		bool RemoveChild(const std::shared_ptr<GameObject>& go);
		void AddChild(const std::weak_ptr<GameObject>& go);

	private:
		std::vector<std::shared_ptr<Component>> m_ComponentPtrs{};

		bool m_PositionIsDirty{ true };
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};

		//glm::vec3 m_LocalScale{};
		//glm::vec3 m_WorldScale{};

		//glm::vec3 m_LocalRotation{};
		//glm::vec3 m_WorldRotation{};

		//TODO: to solve memory leak make child or parent weakptr otherwise
		//sharedptrs will point to each other and keep each other alive
		std::shared_ptr<GameObject> m_pParent{ nullptr };

		// Could be unique if not shared to scene.
		std::vector<std::shared_ptr<GameObject>> m_ChildrenPtrs{};

		void UpdateWorldPosition();
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
