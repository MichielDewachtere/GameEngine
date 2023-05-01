#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Component.h"

namespace real
{
	class ColliderComponent final : public Component
	{
	public:
		explicit ColliderComponent(GameObject* pOwner, const glm::vec2& size);
		explicit ColliderComponent(GameObject* pOwner, float width, float height);
		virtual ~ColliderComponent() override = default;
		ColliderComponent(const Component& other) = delete;
		ColliderComponent operator=(const Component& rhs) = delete;
		ColliderComponent(Component&& other) = delete;
		ColliderComponent operator=(Component&& rhs) = delete;

		void Update() override;
		void DebugRender() const override;

		bool CanRender() const override { return true; }

		void SetPosition(const glm::vec2& pos) { m_Pos = pos; }
		void SetPosition(float x, float y) { SetPosition({ x,y }); }
		glm::vec2 GetPosition() const { return m_Pos; }
		void SetSize(const glm::vec2& size) { m_Size = size; }
		void SetSize(float width, float height) { SetSize({ width, height }); }
		glm::vec2 GetSize() const { return m_Size; }

		/**
		 * \brief 
		 * \param enable 
		 * \param color color values between 0 and 255
		 */
		void EnableDebugRendering(bool enable, const glm::vec4& color = { 0,255,0,255 }) { m_DrawDebug = enable; m_Color = color; }
		/**
		 * \brief The Color of the debug box
		 * \param color values between 0 and 255
		 */
		void SetColor(const glm::vec4& color) { m_Color = color; }

		bool IsOverlapping(const ColliderComponent& other) const;
		bool IsEntireColliderOverlapping(const ColliderComponent& other) const;

		/**
		\brief Determines whether the current collider is overlapping with the expected game object, identified by its tag.
		\param other The collider of the other game object.
		\param expectedTag The tag of the expected game object that is checked for overlap.
		\return This function checks whether the current collider is overlapping with another collider belonging to a game object with the expected tag.
		If the expected game object is found to be overlapping with the current collider, the function returns TRUE. If no overlap is detected,
		or the tags don't match, the function returns FALSE.
		*/
		bool IsOverlappingWith(const ColliderComponent& other, const std::string& expectedTag) const;


	private:
		glm::vec2 m_Pos{};
		glm::vec2 m_Size;

		bool m_DrawDebug{ false };
		glm::vec4 m_Color{ 0,255,0,255 };
	};
}
#endif // COLLIDERCOMPONENT_H