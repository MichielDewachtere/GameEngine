//#include "stdafx.h"
#include "PlayerCharacter.h"

#include <GameTime.h>

#include "TextureComponent.h"
#include "TransformComponent.h"

PlayerCharacter::PlayerCharacter(real::GameObject* pOwner)
	: real::Component(pOwner)
{
}

void PlayerCharacter::Update()
{
	if (m_PepperThrown)
	{
		m_AccuTime += real::Time::GetInstance().GetElapsed();

		if (m_AccuTime >= m_MaxStunTime)
		{
			m_AccuTime = 0;
			m_PepperThrown = false;
			GetOwner()->GetChildAt(1)->SetIsActive(false);
			pepperThrown.Notify(m_PepperThrown);
		}
	}
}

void PlayerCharacter::ThrowPepper()
{
	if (m_PepperThrown)
		return;

	m_PepperThrown = true;
	pepperThrown.Notify(m_PepperThrown);

	const auto pPepperTransform = GetOwner()->GetChildAt(1)->GetComponent<real::TransformComponent>();
	const auto playerPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

	const auto pepperSize = GetOwner()->GetChildAt(1)->GetComponent<real::TextureComponent>()->GetTexture()->GetSize();

	const auto newPos = glm::vec2{ static_cast<float>(pepperSize.x) * m_CurrentDirection.x,
		 static_cast<float>(pepperSize.y) * m_CurrentDirection.y } + playerPos;
	pPepperTransform->SetLocalPosition(newPos);

	GetOwner()->GetChildAt(1)->SetIsActive(true);
}
