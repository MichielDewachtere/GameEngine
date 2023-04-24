#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include "Component.h"
#include "Observer.h"

namespace real
{
	class ScoreDisplay final : public Component, public Observer<>
	{
	public:
		//ScoreDisplay(GameObject* pOwner)
	};
}

#endif // SCOREDISPLAY_H