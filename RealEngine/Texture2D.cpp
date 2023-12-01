#include "Texture2D.h"

void real::Texture2D::SetScale(const float uniformScale)
{
	SetScale(uniformScale, uniformScale);
}

void real::Texture2D::SetScale(const float scaleX, const float scaleY)
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}
