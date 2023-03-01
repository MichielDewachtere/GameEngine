#pragma once
#include <string>
#include <memory>

#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		TextComponent(const std::string& text, std::shared_ptr<Font> font, const std::shared_ptr<GameObject>& pOwner);
		virtual ~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;
		void Render() const;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		bool CanRender() const override { return true; }

	private:
		bool m_needsUpdate;
		std::string m_text;
		//TransformComponent m_transform{};
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
