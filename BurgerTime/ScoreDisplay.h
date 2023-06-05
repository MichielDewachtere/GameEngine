#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <Component.h>
#include <Observer.h>

class ScoreDisplay final : public real::Component, public real::Observer<>
{
public:
	//ScoreDisplay(GameObject* pOwner)
};

#endif // SCOREDISPLAY_H