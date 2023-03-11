#include "Component.h"

dae::GameObject* dae::Component::GetOwner() const
{
	return m_pOwner;
}
