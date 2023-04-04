#include "Achievements.h"

real::Achievements::~Achievements()
{
#ifdef USE_STEAM
	delete steamAchievements;
#endif // USE_STEAM
}
