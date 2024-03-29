﻿#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL_rect.h>
#include <glm/vec4.hpp>

#include "Component.h"

namespace real
{
	class Rectangle final : public Component
	{
	public:
		explicit Rectangle(GameObject* pOwner);
		virtual ~Rectangle() override = default;

		Rectangle(const Rectangle& other) = delete;
		Rectangle& operator=(const Rectangle& rhs) = delete;
		Rectangle(Rectangle&& other) = delete;
		Rectangle& operator=(Rectangle&& rhs) = delete;

		void Update() override {}

		bool CanRender() const override { return true; }
		void Render() const override;

		void SetRect(glm::ivec4 rect);
		void SetRect(int x, int y, int width, int height);

		glm::ivec4 GetRectangle() const { return m_Rectangle; }

		void SetIsFilled(bool isFilled) { m_IsFilled = isFilled; }
		void SetColor(glm::u8vec4 color) { m_Color = color; }

	private:
		glm::ivec4 m_Rectangle{};
		bool m_IsFilled{ true };
		glm::u8vec4 m_Color{ 255,255,255,255 };
	};
}

#endif // RECTANGLE_H