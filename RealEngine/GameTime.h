#ifndef TIME_H
#define TIME_H

#include <chrono>

#include "Singleton.h"

namespace real
{
	class Time final : public Singleton<Time>
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

#endif // TIME_H