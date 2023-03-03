#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Time final : public dae::Singleton<Time>
	{
	public:
		void Init();
		void Update();

		float GetElapsed() const;
		float GetTotal() const;

	private:
		std::chrono::steady_clock::time_point m_PrevTime{};
		float m_DeltaTime{};
		float m_TotalTime{};	
	};
}