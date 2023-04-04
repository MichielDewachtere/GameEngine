#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "stdafx.h"

#include "Component.h"

namespace real
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* pOwner) : Component(pOwner) {}
		virtual ~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		std::string GetText() const { return m_pText; }

		void SetFont(const std::shared_ptr<Font>& pFont);

		void SetColor(const SDL_Color& color);
		void SetColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a = 0);

		bool CanRender() const override { return false; }

	private:
		bool m_NeedsUpdate{ true };
		std::string m_pText{"empty"};
		std::shared_ptr<Font> m_pFont{};
		SDL_Color m_Color{ 255,255,255 };
	};
}

#endif // TEXTCOMPONENT_H