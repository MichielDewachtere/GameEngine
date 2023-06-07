#include "Level02.h"

#include <TransformComponent.h>

#include "LevelParser.h"
#include "GameInfo.h"

Level02::Level02()
	: Scene(Scenes::level02, InputMaps::gameplay)
{
}

void Level02::Load()
{
	this->SetDebugRendering(true);

	const auto pLevel = LevelParser::ParseLevel(*this, "Level02.json");
	pLevel->GetComponent<real::TransformComponent>()->Translate(48, 62);

	m_IsLoaded = true;
}
