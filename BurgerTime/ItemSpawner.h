﻿#ifndef ITEMSPAWNER_H
#define ITEMSPAWNER_H

#include <Component.h>

#include "Subject.h"

class ItemSpawner final :
	public real::Component,
	public real::Observer<>,
	public real::Observer<real::Scene&>
{
public:
	explicit ItemSpawner(real::GameObject* pOwner);
	virtual ~ItemSpawner() override;
	ItemSpawner(const ItemSpawner& other) = delete;
	ItemSpawner& operator=(const ItemSpawner& other) = delete;
	ItemSpawner(ItemSpawner&& other) = delete;
	ItemSpawner& operator=(ItemSpawner&& other) = delete;

	void Start() override;
	void Update() override;
	bool CanRender() const override { return false; }

	void HandleEvent() override;
	void HandleEvent(real::Scene&) override;
	void OnSubjectDestroy() override {}

	real::Subject<bool> isItemSpawned;
private:
	bool m_IsActive{};
	float m_AccuTime{};
	float m_LifeTime{ 6 };

	void SetComponentsActive(bool enable);
	void RemoveObservers();
};

#endif // ITEMSPAWNER_H