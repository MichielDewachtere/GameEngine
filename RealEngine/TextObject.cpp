#include "TextObject.h"

#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

void real::TextObject::InitComponents(glm::vec2 pos, std::shared_ptr<Font> pFont, std::string text)
{
	this->GetComponent<TransformComponent>()->SetLocalPosition(pos);
	this->GetComponent<TextComponent>()->SetFont(pFont);
	this->GetComponent<TextComponent>()->SetText(text);
}

void real::TextObject::Init()
{
	GameObject::Init();

	AddComponent<TextureComponent>();
	AddComponent<TextComponent>();
}
