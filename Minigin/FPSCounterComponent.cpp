#include "FPSCounterComponent.h"

#include "TextComponent.h"
#include "Time.h"

void dae::FPSCounterComponent::Update()
{
	const auto pTextComponent = this->GetOwner()/*.lock()*/->GetComponent<TextComponent>();

	if (!pTextComponent)
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
