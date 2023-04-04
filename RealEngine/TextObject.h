#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "stdafx.h"

#include "GameObject.h"

namespace real
{
	class Font;

	class TextObject final : public GameObject
    {
    public:
		explicit TextObject() = default;
		explicit TextObject(Scene* pScene) : GameObject(pScene) {}
		virtual ~TextObject() override = default;
		TextObject(const TextObject& other) = delete;
		TextObject& operator=(const TextObject& rhs) = delete;
		TextObject(TextObject&& other) = delete;
		TextObject& operator=(TextObject&& rhs) = delete;

		//virtual GameObject* CreateGameObject() override;
    	void InitComponents(glm::vec2 pos, std::shared_ptr<Font> pFont, std::string text);

		virtual void Init() override;
    };
}

#endif // TEXTOBJECT_H