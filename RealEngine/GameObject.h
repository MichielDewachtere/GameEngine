#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <vector>
#include <stdexcept>

//#include "TransformComponent.h"
#include "Component.h"

namespace real
{
	//class TransformComponent;
	class Scene;
	class Texture2D;
	class Component;

	using gameobject_id = unsigned short;

	class GameObject final
	{
	public:
		explicit GameObject() = default;
		explicit GameObject(Scene* pScene, const std::string& tag = "empty") 
			: m_pScene(pScene), m_Tag(tag), m_Id(++m_NextId) {}
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

		GameObject* CreateGameObject();
		std::vector<GameObject*> GetObjectsWithTag(const std::string& tag);
		GameObject* GetObject(gameobject_id id);

		//void Init();

		void Start();
		void Update();
		void Render() const;
		void DebugRender() const;
		void PostUpdate();

		void Destroy();
		bool IsMarkedForDestroy() const { return m_IsMarkedForDestroy; }

		void SetTag(const std::string& newTag) { m_Tag = newTag; }
		std::string GetTag() const { return m_Tag; }
		gameobject_id GetId() const { return m_Id; }

		bool GetIsActive() const { return m_IsActive; }
		void SetIsActive(const bool value);

		void SetCanBeDestroyed(const bool value) { m_CanBeDestroyed = value; }

		//Component Logic
		template <class T, typename... Args>
		T* AddComponent(Args&&... args);
		template <class T>
		T* GetComponent();
		template <class T>
		void RemoveComponent();
		template <class T>
		void RemoveComponent(const T&);
		template <class T>
		bool HasComponent() const;

		//Child-Parent Logic
		void SetParent(GameObject* pParent, const bool keepWorldPosition);
		GameObject* GetParent() const { return m_pParent; }

		size_t GetChildCount() const { return m_ChildrenPtrs.size(); }
		GameObject* GetChildAt(const unsigned int idx) const { return m_ChildrenPtrs[idx].get(); }
		std::vector<GameObject*> GetChildren() const;

	private:
		Scene* m_pScene{ nullptr };
		std::string m_Tag;
		//TransformComponent* m_pTransform{ nullptr };
		//std::unique_ptr<TransformComponent> m_pTransform;
		bool m_IsMarkedForDestroy{};
		bool m_IsActive{ true };
		bool m_CanBeDestroyed{ true };

		std::vector<std::unique_ptr<Component>> m_ComponentPtrs{};

		GameObject* m_pParent{ nullptr };
		std::vector<std::unique_ptr<GameObject>> m_ChildrenPtrs{};

		gameobject_id m_Id;
		static inline gameobject_id m_NextId = 0;

		void RemoveChild(std::unique_ptr<GameObject> gameObject);
	};

#pragma region ComponentLogic
	template <class T, typename... Args>
	T* GameObject::AddComponent(Args&&... args)
	{
		if (HasComponent<T>())
			throw std::runtime_error("Error: Could not add component to GameObject - a component of this type is already attached. Please remove the existing component before adding another.");

		std::unique_ptr<T> pComponent = std::make_unique<T>(this, std::forward<Args>(args)...);

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

		for (auto it = m_ComponentPtrs.begin(); it != m_ComponentPtrs.end(); /*++it*/)
		{
			//T* otherComponent = dynamic_cast<T*>(*it->get());
			//if (otherComponent != nullptr)
			//{
			//	it = m_ComponentPtrs.erase(it);
			//	break;
			//}

			T* otherComponent = dynamic_cast<T*>(it->get());
			if (otherComponent != nullptr)
			{
				it = m_ComponentPtrs.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	template <class T>
	void GameObject::RemoveComponent(const T&)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

		RemoveComponent<T>();
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
}

#endif // GAMEOBJECT_H