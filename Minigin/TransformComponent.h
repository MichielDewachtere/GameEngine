#pragma once
#include <glm/glm.hpp>

#include "Component.h"

namespace dae
{
	class GameObject;
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* pOwner) : Component(pOwner) {}
		~TransformComponent() override = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent& operator=(const TransformComponent& rhs) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(TransformComponent&& rhs) = delete;

		void Update() override;

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();

		void SetWorldPosition(float x, float y, float z = 0);
		void SetWorldPosition(const glm::vec3& pos);

		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& pos);

		bool CanRender() const override { return false; }
	private:
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};

		bool m_PositionIsDirty = true;

		void UpdateWorldPosition();
		void SetPositionDirty();
	};
}