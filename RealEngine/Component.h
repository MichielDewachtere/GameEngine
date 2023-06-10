#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"

namespace real
{
	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component& operator=(const Component& rhs) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component&& rhs) = delete;

		virtual void Start() {}
		virtual void Update() = 0;
		virtual void PostUpdate() {}
		virtual void Render() const {}
		virtual void DebugRender() const {}
		//virtual void FixedUpdate() = 0;

		virtual bool CanRender() const = 0;

		bool GetIsActive() const { return m_IsActive; }
		void SetIsActive(const bool value) { m_IsActive = value; }

		GameObject* GetOwner() const { return m_pOwner/*.get()*/; }

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}

	private:
		//std::unique_ptr<GameObject> m_pOwner{};
		GameObject* m_pOwner{};
		bool m_IsActive{ true };
	};
}

#endif // COMPONENT_H