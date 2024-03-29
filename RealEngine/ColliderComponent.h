#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Component.h"
#include "Observer.h"

namespace real
{
	class ColliderComponent final
		: public Component
		, public Observer<glm::ivec2>
	{
	public:
		explicit ColliderComponent(GameObject* pOwner, const glm::vec2& size);
		explicit ColliderComponent(GameObject* pOwner, float width, float height);
		virtual ~ColliderComponent() override;
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent operator=(const ColliderComponent& rhs) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent operator=(ColliderComponent&& rhs) = delete;

		void Start() override;
		void Update() override;
		void DebugRender() const override;

		void HandleEvent(glm::ivec2) override;
		void OnSubjectDestroy() override {}

		bool CanRender() const override { return true; }

		void SetPosition(const glm::vec2& pos) { m_Pos = pos; }
		void SetPosition(float x, float y) { SetPosition({ x,y }); }
		void Translate(float x, float y) { Translate({ x,y }); }
		void Translate(const glm::vec2& t) { m_Offset = t; m_IsDirty = true; }
		glm::vec2 GetPosition() const { return m_Pos/* + m_Offset*/; }
		void SetSize(const glm::vec2& size);
		void SetSize(float width, float height) { SetSize({ width, height }); }
		glm::vec2 GetSize() const { return m_Size; }

		void EnableDebugRendering(bool enable, const glm::u8vec4& color = { 0,255,0,255 }) { m_DrawDebug = enable; m_Color = color; }
		/**
		 * \brief The Color of the debug box
		 * \param color values between 0 and 255
		 */
		void SetColor(const glm::u8vec4& color) { m_Color = color; }

		bool IsOverlapping(const ColliderComponent& other) const;
		bool IsEntireColliderOverlapping(const ColliderComponent& other, const glm::vec2& offset = { 0,0 }) const;

		/**
		\brief Determines whether the current collider is overlapping with the expected game object, identified by its tag.
		\param other The collider of the other game object.
		\param expectedTag The tag of the expected game object that is checked for overlap.
		\return This function checks whether the current collider is overlapping with another collider belonging to a game object with the expected tag.
		If the expected game object is found to be overlapping with the current collider, the function returns TRUE. If no overlap is detected,
		or the tags don't match, the function returns FALSE.
		*/
		bool IsOverlappingWith(const ColliderComponent& other, const std::string& expectedTag) const;
		//bool IsOverlappingOrUnder(const ColliderComponent& other);
		bool IsMouseOverlapping() const;

	private:
		glm::vec2 m_Pos{}, m_Offset{}, m_Size{};
		bool m_IsDirty{ false }, m_DrawDebug{ false };
		glm::u8vec4 m_Color{ 0,255,0,255 };
	};
}
#endif // COLLIDERCOMPONENT_H