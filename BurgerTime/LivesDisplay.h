#ifndef LIFESDISPLAY_H
#define LIFESDISPLAY_H

#include <Observer.h>
#include <Component.h>
#include <SpriteComponent.h>

class HealthComponent;

class LivesDisplay final : public real::Component,
	public real::Observer<bool>,
	public real::Observer<>
{
public:
	explicit LivesDisplay(real::GameObject* pOwner);
	virtual ~LivesDisplay() override;
	LivesDisplay(const LivesDisplay& other) = delete;
	LivesDisplay operator=(const LivesDisplay& rhs) = delete;
	LivesDisplay(LivesDisplay&& other) = delete;
	LivesDisplay operator=(LivesDisplay&& rhs) = delete;

	void Start() override;
	void Update() override {}
	bool CanRender() const override { return false; }

private:
	int m_LifeIcons{ 3 };
	real::SpriteSheet m_SpriteSheet{};
	int m_AmountOfPlayers{ 1 };
	bool m_SubjectDestroyed{};

	void HandleEvent() override;
	virtual void HandleEvent(bool) override;
	virtual void OnSubjectDestroy() override { m_SubjectDestroyed = true; }

	void InitLifeIcons(int players);
};

#endif // LIFESDISPLAY_H