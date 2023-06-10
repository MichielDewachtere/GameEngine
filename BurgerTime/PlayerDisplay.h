#pragma once

#include <Component.h>
#include <Observer.h>
#include <Minigin.h>
#include <glm/vec2.hpp>

class PlayerDisplay final : public real::Component,
                            public real::Observer<int>
{
public:
	PlayerDisplay(real::GameObject* pOwner);
	virtual ~PlayerDisplay() override;

	void Update() override {}
	bool CanRender() const override { return false; }

	virtual void HandleEvent(int) override;
	virtual void OnSubjectDestroy() override {}

private:
	real::WindowSettings m_Settings;
	glm::vec2 m_Size{};
};
