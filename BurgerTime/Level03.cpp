#include "Level03.h"

#include <TransformComponent.h>

#include "LevelParser.h"
#include "GameInfo.h"

Level03::Level03()
	: Scene(Scenes::level03, InputMaps::gameplay)
{
}

void Level03::Load()
{
	this->SetDebugRendering(true);

	const auto pLevel = LevelParser::ParseLevel(*this, "Level03.json");
	pLevel->GetComponent<real::TransformComponent>()->Translate(48, 62);

	m_IsLoaded = true;
}

