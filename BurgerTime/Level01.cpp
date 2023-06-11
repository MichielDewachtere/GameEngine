#include "Level01.h"

#include <TransformComponent.h>

#include "LevelParser.h"
#include "GameInfo.h"

Level01::Level01()
	: Scene(Scenes::level01, InputMaps::gameplay)
{
}

void Level01::Load()
{
	this->SetDebugRendering(true);

	const auto pLevel = LevelParser::ParseLevel(*this, "Level01.json");
	pLevel->GetComponent<real::TransformComponent>()->Translate(48, 62);

	m_IsLoaded = true;
}
