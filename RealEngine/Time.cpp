#include "stdafx.h"
#include "Time.h"

void real::Time::Init()
{
	m_PrevTime = std::chrono::high_resolution_clock::now();
}

void real::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration<float>(currentTime - m_PrevTime).count();
	m_TotalTime += m_DeltaTime;

	m_PrevTime = currentTime;
}

float real::Time::GetElapsed() const
{
	return m_DeltaTime;
}

float real::Time::GetTotal() const
{
	return m_TotalTime;
}
