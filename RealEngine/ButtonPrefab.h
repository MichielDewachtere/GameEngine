#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Prefab.h"

namespace real
{
	class Font;
	class Texture2D;

	class ButtonPrefab final : public Prefab
	{
	public:
		explicit ButtonPrefab(Scene& scene, glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Font> font);
		explicit ButtonPrefab(Scene& scene, glm::ivec2 pos, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font);
		explicit ButtonPrefab(GameObject& gameObject, glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Font> font);
		explicit ButtonPrefab(GameObject& gameObject, glm::ivec2 pos, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font);
		virtual ~ButtonPrefab() override = default;

		ButtonPrefab& SetText(const std::string& text);
		ButtonPrefab& SetText(const std::string& text, const glm::u8vec4& color);
		ButtonPrefab& SetFont(std::shared_ptr<Font> font);
		ButtonPrefab& SetTexture(std::shared_ptr<Texture2D> texture);
		ButtonPrefab& SetColor(const glm::u8vec4& color);

		// TODO: add functionality

	private:
		void InitComponents(glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font) const;
	};

	template <class T, typename ... Args>
	ButtonPrefab& ButtonPrefab::OnButtonPress(uint8_t commandId, real::InputMap& inputMap, Args... commandArgs)
	{
		inputMap.AddMouseInput<T>(commandId, SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT, m_pGameObject, commandArgs...);
		return *this;
	}
}

#endif // BUTTON_H