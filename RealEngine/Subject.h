#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

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
				observer->OnSubjectDestroy();
			}
		}

		void AddObserver(Observer<Args...>* pObserver)
		{
			m_ObserverPtrs.push_back(pObserver);
		}

		void RemoveObserver(Observer<Args...>* pObserver)
		{
			m_ObserverPtrs.erase(std::remove(
				m_ObserverPtrs.begin(),
				m_ObserverPtrs.end(), pObserver),
				m_ObserverPtrs.end());
		}

		void Notify(Args... args)
		{
			for (auto& pObserver : m_ObserverPtrs)
			{
				pObserver->HandleEvent(args...);
			}
		}

	private:
		std::vector<Observer<Args ...>*> m_ObserverPtrs;
	};
}

#endif // SUBJECT_H