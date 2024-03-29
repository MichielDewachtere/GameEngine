﻿#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

//==================================
//	This is based on Mendel Debrabandere's SpriteComponent
//	Github: https://github.com/MendelDebrabandere/GameEngineProg4/blob/main/Minigin/SpriteComponent.h
//==================================

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Component.h"
#include "Renderer.h"
#include "Texture2D.h"

namespace real
{
	struct SpriteSheet
	{
		std::shared_ptr<Texture2D> pTexture{};
		int rows{}, columns{};
		float animTimer{};
	};

	class SpriteComponent final : public Component
	{
	public:
		explicit SpriteComponent(GameObject* pOwner, SpriteSheet spriteSheet);
		virtual ~SpriteComponent() override = default;
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) = delete;

		void Update() override;
		void Render() const override;
		bool CanRender() const override { return !m_Stop; }

		void SelectSprite(int idx);

		void PlayAnimation(int startIdx, int endIdx, int loops = -1);
		void PlayAnimation(const std::vector<int>& idcs, int loops = -1);
		void UpdateAnimation(int startIdx, int endIdx, int loops = -1);
		bool IsAnimationPlaying() const { return m_IsAnimationPlaying; }
		void Pause(bool value);
		void Stop(bool value);
		void FlipTexture(RenderFlip flip);

		glm::ivec2 GetSpriteSize() const; 

	private:
		SpriteSheet m_SpriteSheet;

		int m_CurrIdx{};
		int m_StartIdx{};
		int m_EndIdx{};
		bool m_PlaySelectedIdcs{};
		std::vector<int> m_SelectedIdcs{};

		int m_Loops{};
		int m_AccuLoops{};

		float m_AccuTime{};

		bool m_Pause{};
		bool m_IsAnimationPlaying{};
		bool m_Stop{};

		glm::vec2 m_RenderPos{};
		glm::ivec4 m_Rect{};
		RenderFlip m_Flip{ RenderFlip::none };

		//std::shared_ptr<SDLTexture2D> m_Texture{};
	};
}
#endif // SPRITECOMPONENT_H