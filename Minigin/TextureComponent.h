#pragma once
#include <memory>

#include "Component.h"
#include "Texture2D.h"

namespace dae
{
	class TextureComponent final : public Component
	{
	public:
		TextureComponent() = default;
		explicit TextureComponent(const std::shared_ptr<GameObject>& pOwner) : Component(pOwner) {}
		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent& operator=(const TextureComponent& rhs) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(TextureComponent&& rhs) = delete;

		void Update() override {};
		void Render() const override;

		void SetTexture(const std::shared_ptr<Texture2D> & pTexture) { m_pTexture = pTexture; }

		bool CanRender() const override { return true; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}