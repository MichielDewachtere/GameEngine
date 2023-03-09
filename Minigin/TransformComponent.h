#pragma once
#include <glm/glm.hpp>

#include "Component.h"

namespace dae
{
	class GameObject;
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(const std::weak_ptr<GameObject>& pOwner) : Component(pOwner) {}
		~TransformComponent() override = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent& operator=(const TransformComponent& rhs) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(TransformComponent&& rhs) = delete;

		void Update() override;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& pos) { m_Position = pos; }

		bool CanRender() const override { return false; }
	private:
		glm::vec3 m_Position{};
	};
}