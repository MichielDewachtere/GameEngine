#include "Component.h"

std::weak_ptr<dae::GameObject> dae::Component::GetOwner() const
{
	return m_pOwner;
}
