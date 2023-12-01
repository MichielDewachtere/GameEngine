//#include "stdafx.h"
#include "Locator.h"

void real::Locator::RegisterAudioSystem(Audio* audio)
{
	if (audio == nullptr)
		m_AudioSystem.reset(&m_DefaultAudioSystem);
	else
		m_AudioSystem.reset(audio);
}

void real::Locator::RegisterRenderSystem(Renderer* renderer)
{
	if (renderer == nullptr)
		m_RenderSystem.reset(&m_DefaultRenderSystem);
	else
		m_RenderSystem.reset(renderer);
}

void real::Locator::RegisterResourceSystem(ResourceManager* resourceManager)
{
	if (resourceManager == nullptr)
		m_RenderSystem.reset(&m_DefaultRenderSystem);
	else
		m_ResourceSystem.reset(resourceManager);
}

void real::Locator::ShutDown()
{
	m_AudioSystem.reset(&m_DefaultAudioSystem);
	m_RenderSystem.reset(&m_DefaultRenderSystem);
	m_ResourceSystem.reset(&m_DefaultResourceSystem);
}
