#include "Component.h"

real::GameObject* real::Component::GetOwner() const
{
	return m_pOwner;
}
