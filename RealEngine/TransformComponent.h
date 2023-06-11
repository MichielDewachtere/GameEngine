#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/vec2.hpp>

#include "Component.h"

namespace real
{
	class GameObject;
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* pOwner) : Component(pOwner) {}
		virtual ~TransformComponent() override = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent& operator=(const TransformComponent& rhs) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(TransformComponent&& rhs) = delete;

		void Update() override;

		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetWorldPosition();

		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const glm::vec2& pos);

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& pos);

		void Translate(float x, float y);
		void Translate(const glm::vec2& translation);

		void SetIgnoreParent(bool ignoreParent) { m_IgnoreParent = ignoreParent; }
		bool GetIgnoreParent() const { return m_IgnoreParent; }

		bool CanRender() const override { return false; }

	private:
		glm::vec2 m_LocalPosition{};
		glm::vec2 m_WorldPosition{};

		bool m_PositionIsDirty = true;
		bool m_IgnoreParent{};

		void UpdateWorldPosition();
		void SetPositionDirty();
	};
}

#endif // TRANSFORMCOMPONENT_H