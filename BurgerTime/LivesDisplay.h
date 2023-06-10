#ifndef LIFESDISPLAY_H
#define LIFESDISPLAY_H

#include <Observer.h>
#include <Component.h>

class HealthComponent;

class LivesDisplay final : public real::Component,
	public real::Observer<>
{
public:
	LivesDisplay(real::GameObject* pOwner);
	virtual ~LivesDisplay() override;
	LivesDisplay(const Component& other) = delete;
	LivesDisplay operator=(const Component& rhs) = delete;
	LivesDisplay(Component&& other) = delete;
	LivesDisplay operator=(Component&& rhs) = delete;

	void Update() override {}
	bool CanRender() const override { return false; }

private:
	int m_LivesLost{0};

	virtual void HandleEvent() override;
	virtual void OnSubjectDestroy() override {}

	void InitLifeIcons(int players);
};

#endif // LIFESDISPLAY_H