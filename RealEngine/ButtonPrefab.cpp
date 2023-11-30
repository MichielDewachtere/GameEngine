#include "ButtonPrefab.h"

#include "Scene.h"

#include "TransformComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "Font.h"
#include "ColliderComponent.h"
#include "Rectangle.h"

real::ButtonPrefab::ButtonPrefab(Scene& scene, glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Font> font)
	: Prefab(scene)
{
	InitComponents(pos, size, nullptr, std::move(font));
}
real::ButtonPrefab::ButtonPrefab(Scene& scene, glm::ivec2 pos, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font)
	: Prefab(scene)
{
	const auto size = texture->GetSize();
	InitComponents(pos, size, std::move(texture), std::move(font));
}
real::ButtonPrefab::ButtonPrefab(GameObject& gameObject, glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Font> font)
	: Prefab(gameObject)
{
	InitComponents(pos, size, nullptr, std::move(font));
}
real::ButtonPrefab::ButtonPrefab(GameObject& gameObject, glm::ivec2 pos, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font)
	: Prefab(gameObject)
{
	const auto size = texture->GetSize();
	InitComponents(pos, size, std::move(texture), std::move(font));
}

real::ButtonPrefab& real::ButtonPrefab::SetText(const std::string& text)
{
	m_pGameObject->GetComponent<TextComponent>()->SetText(text);

	return *this;
}

real::ButtonPrefab& real::ButtonPrefab::SetText(const std::string& text, const glm::u8vec4& color)
{
	m_pGameObject->GetComponent<TextComponent>()->SetColor(color);
	return SetText(text);
}

real::ButtonPrefab& real::ButtonPrefab::SetFont(std::shared_ptr<Font> font)
{
	m_pGameObject->GetComponent<TextComponent>()->SetFont(std::move(font));
	return *this;
}

real::ButtonPrefab& real::ButtonPrefab::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_pGameObject->GetComponent<TextureComponent>()->SetTexture(std::move(texture));

	if (m_pGameObject->HasComponent<Rectangle>())
		m_pGameObject->RemoveComponent<Rectangle>();

	return *this;
}

real::ButtonPrefab& real::ButtonPrefab::SetColor(const glm::u8vec4& color)
{
	if (m_pGameObject->HasComponent<Rectangle>())
	{
		m_pGameObject->GetComponent<Rectangle>()->SetColor({ color.r, color.g, color.b, color.a });
	}

	return *this;
}

void real::ButtonPrefab::InitComponents(glm::ivec2 pos, glm::ivec2 size, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font) const
{
	m_pGameObject->GetComponent<TransformComponent>()->SetLocalPosition(pos);
	m_pGameObject->AddComponent<TextureComponent>()->SetRenderOffset(size.x / 2, size.y / 2);
	m_pGameObject->AddComponent<ColliderComponent>(size)->EnableDebugRendering(true);
	m_pGameObject->GetComponent<ColliderComponent>()->Translate(-(size.x / 2.f), -(size.y / 2.f));

	if (texture != nullptr)
		m_pGameObject->GetComponent<TextureComponent>()->SetTexture(std::move(texture));
	else
	{
		m_pGameObject->AddComponent<Rectangle>()->SetRect({ pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y });
		m_pGameObject->GetComponent<Rectangle>()->SetColor({ 40,40,40,255 });
	}

	m_pGameObject->AddComponent<TextComponent>()->SetFont(std::move(font));
	m_pGameObject->GetComponent<TextComponent>()->SetText("Button");
	m_pGameObject->GetComponent<TextComponent>()->ChangeVerticalAlignment(TextComponent::VerticalAlignment::center);
	m_pGameObject->GetComponent<TextComponent>()->ChangeHorizontalAlignment(TextComponent::HorizontalAlignment::center);
}
