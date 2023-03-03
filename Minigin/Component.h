#pragma once
#include "GameObject.h"

namespace dae
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
		std::weak_ptr<dae::GameObject> GetOwner() const;

		explicit Component(std::weak_ptr<GameObject> pOwner) : m_pOwner(std::move(pOwner)) {}

	private:
		std::weak_ptr<GameObject> m_pOwner{ };
	};
}
