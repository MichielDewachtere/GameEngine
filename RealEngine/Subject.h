#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <algorithm>

#include "Observer.h"
#include "Logger.h"

namespace real
{
	template<typename... Args>
	class Subject final
	{
	public:
		~Subject()
		{
			for (auto& observer : m_ObserverPtrs)
			{
				Observer<Args ...>* observerPtr = reinterpret_cast<Observer<Args ...>*>(observer);

				if (observerPtr == nullptr)
					continue;

				observerPtr->OnSubjectDestroy();
			}
			m_ObserverPtrs.clear();
		}

		void AddObserver(Observer<Args...>* pObserver)
		{
			if (std::find(m_ObserverPtrs.begin(), m_ObserverPtrs.end(), pObserver) != m_ObserverPtrs.end())
			{
				Logger::LogWarning("Observer {} is already added as listener to subject {}", pObserver, this);
				return;
			}

			m_ObserverPtrs.push_back(pObserver);
		}

		bool HasObserver(Observer<Args...>* pObserver)
		{
			const auto it = std::ranges::find(m_ObserverPtrs, pObserver);

			if (it == m_ObserverPtrs.end())
				return false;

			return true;
		}

		void RemoveObserver(Observer<Args...>* pObserver)
		{
			if (m_ObserverPtrs.empty() == false)
			{
				const auto removeIt = std::remove(m_ObserverPtrs.begin(), m_ObserverPtrs.end(), pObserver);
				
				if (removeIt == m_ObserverPtrs.end())
				{
					//m_ObserverPtrs.erase(m_ObserverPtrs.begin());
					Logger::LogWarning("Subject.h => Could not remove observer {}", pObserver);
				}
				else
				{
					// for some reason in BurgerTime the last Base Enemy defined cant delete their observers...
					std::erase(m_ObserverPtrs, pObserver);
				}

				//m_ObserverPtrs.erase(std::remove(
				//	m_ObserverPtrs.begin(),
				//	m_ObserverPtrs.end(), pObserver),
				//	m_ObserverPtrs.end());
			}
		}

		void Notify(Args... args)
		{
 			for (auto& pObserver : m_ObserverPtrs)
			{
				if (pObserver == nullptr)
					std::erase(m_ObserverPtrs, pObserver);

				pObserver->HandleEvent(args...);
			}
		}

		std::vector<Observer<Args ...>*> GetObservers() const { return m_ObserverPtrs; }

	private:
		std::vector<Observer<Args ...>*> m_ObserverPtrs;
	};
}

#endif // SUBJECT_H