#ifndef OBSERVER_H
#define OBSERVER_H

namespace real
{
	class GameObject;

	class Observer
	{
	public:
		enum class GameEvent
		{
			actorTookDamage,
			actorDied,
			actorGainedPoints
		};

		explicit Observer() = default;
		virtual ~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(Observer&& other) = delete;

		virtual void Notify(GameEvent event, GameObject* actor) = 0;
	};
}

#endif // OBSERVER_H