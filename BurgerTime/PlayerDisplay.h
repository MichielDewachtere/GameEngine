#ifndef PLAYERDISPLAY_H
#define PLAYERDISPLAY_H

#include <Component.h>
#include <Observer.h>
#include <Minigin.h>

class PlayerDisplay final : public real::Component,
                            public real::Observer<int>
{
public:
	explicit PlayerDisplay(real::GameObject* pOwner);
	virtual ~PlayerDisplay() override;
	PlayerDisplay(const PlayerDisplay& other) = delete;
	PlayerDisplay operator=(const PlayerDisplay& rhs) = delete;
	PlayerDisplay(PlayerDisplay&& other) = delete;
	PlayerDisplay operator=(PlayerDisplay&& rhs) = delete;

	void Update() override {}
	bool CanRender() const override { return false; }

	virtual void HandleEvent(int) override;
	virtual void OnSubjectDestroy() override {}

private:
	real::WindowSettings m_Settings;
};

#endif // PLAYERDISPLAY_H