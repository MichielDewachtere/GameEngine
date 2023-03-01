#pragma once
#include "GameObject.h"

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component() = default;
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
		std::shared_ptr<GameObject> GetOwner() const { return m_pOwner; }
		explicit Component(const std::shared_ptr<GameObject>& pOwner) : m_pOwner(pOwner) {}

	private:
		std::shared_ptr<GameObject> m_pOwner;
	};
}