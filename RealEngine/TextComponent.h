#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <SDL.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Component.h"

namespace real
{
	class Font;

	class TextComponent final : public Component
	{
	public:
		enum class HorizontalAlignment
		{
			left,
			center,
			right
		};
		enum class VerticalAlignment
		{
			down,
			center,
			up
		};

		explicit TextComponent(GameObject* pOwner);
		virtual ~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		std::string GetText() const { return m_pText; }

		void SetFont(const std::shared_ptr<Font>& pFont);

		void SetColor(const glm::u8vec4& color);
		void SetColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0);

		void ChangeHorizontalAlignment(HorizontalAlignment newAlignment);
		void ChangeVerticalAlignment(VerticalAlignment newAlignment);

		bool CanRender() const override { return false; }

	private:
		bool m_NeedsUpdate{ true };
		std::string m_pText{"empty"};
		std::shared_ptr<Font> m_pFont{};
		glm::u8vec4 m_Color{ 255,255,255,255 };

		glm::vec2 m_OriginalPos{};

		HorizontalAlignment m_CurHorizontalAlignment{ HorizontalAlignment::right };
		VerticalAlignment m_CurVerticalAlignment{ VerticalAlignment::down };

		void HandleHorizontalAlignment(const glm::vec2&, glm::ivec2& renderOffset) const;
		void HandleVerticalAlignment(const glm::vec2&, glm::ivec2&) const;
	};
}
#endif // TEXTCOMPONENT_H