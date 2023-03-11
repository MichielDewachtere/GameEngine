#pragma once
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

		virtual void Update() = 0;
		virtual void Render() const {};
		//virtual void FixedUpdate() = 0;

		virtual bool CanRender() const = 0;

	protected:
		//std::weak_ptr<real::GameObject> GetOwner() const;
		GameObject* GetOwner() const;

		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}

	private:
		GameObject* m_pOwner{ };
	};
}
