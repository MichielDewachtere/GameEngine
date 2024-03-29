//#include "stdafx.h"
#include "FPSCounterComponent.h"

#include <TextComponent.h>
#include <GameTime.h>

void FPSCounterComponent::Update()
{
	using namespace real;

	const auto pTextComponent = this->GetOwner()->GetComponent<TextComponent>();

	if (pTextComponent == nullptr)
		return;

	const float dt = Time::GetInstance().GetElapsed();
	m_Time += dt;

	if (m_Time >= m_UpdateInterval)
	{
		const int fps = int(1 / dt);

		pTextComponent->SetText("FPS: " + std::to_string(fps));

		m_Time = 0;
	}
}
