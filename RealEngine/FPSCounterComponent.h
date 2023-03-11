#pragma once
#include "Component.h"

namespace real
{
	class FPSCounterComponent final : public Component
	{
	public:
		explicit FPSCounterComponent(GameObject* pOwner) : Component(pOwner) {}
		virtual ~FPSCounterComponent() override = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;

		void Update() override;

		bool CanRender() const override { return false; }
	private:
		const float m_UpdateInterval{ 0.1f };
		float m_Time{ m_UpdateInterval };
	};
}