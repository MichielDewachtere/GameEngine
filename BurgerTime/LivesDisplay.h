#ifndef LIVESDISPLAY_H
#define LIVESDISPLAY_H

#include "Observer.h"
#include "Component.h"

class HealthComponent;

class LivesDisplay final : public real::Component,
	public real::Observer<>
{
public:
	LivesDisplay(real::GameObject* pOwner, HealthComponent* pHealth);
	virtual ~LivesDisplay() override;

	void Update() override {};
	bool CanRender() const override { return false; }

private:
	HealthComponent* m_Health{};

	virtual void HandleEvent() override;
	virtual void OnSubjectDestroy() override {}

	void UpdateLivesText() const;
};

#endif // LIVESDISPLAY_H