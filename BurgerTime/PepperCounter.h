#ifndef PEPPERCOUNTER_H
#define PEPPERCOUNTER_H

#include <Observer.h>
#include <Component.h>

class HealthComponent;

class PepperCounter final : public real::Component,
	public real::Observer<int>
{
public:
	explicit PepperCounter(real::GameObject* pOwner);
	virtual ~PepperCounter() override;
	PepperCounter(const PepperCounter& other) = delete;
	PepperCounter operator=(const PepperCounter& rhs) = delete;
	PepperCounter(PepperCounter&& other) = delete;
	PepperCounter operator=(PepperCounter&& rhs) = delete;

	void Start() override;
	void Update() override {};
	bool CanRender() const override { return false; }

	virtual void HandleEvent(int) override;
	virtual void OnSubjectDestroy() override {}

private:
	void UpdatePepperText(int);
};

#endif // PEPPERCOUNTER_H