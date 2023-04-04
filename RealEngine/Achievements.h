#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Singleton.h"

#ifdef USE_STEAM
#include "SteamAchievements.h"
#endif // USE_STEAM

namespace real
{
	class Achievements final : public Singleton<Achievements>
	{
	public:
		Achievements() = default;
		virtual ~Achievements() override;
		Achievements(const Achievements& other) = delete;
		Achievements& operator=(const Achievements& other) = delete;
		Achievements(Achievements&& other) = delete;
		Achievements& operator=(Achievements&& other) = delete;

#pragma region SteamAchievenemts
		// Defining our achievements
		enum EAchievements
		{
			ACH_WIN_ONE_GAME = 0,
			ACH_WIN_100_GAMES = 1,
			ACH_TRAVEL_FAR_ACCUM = 2,
			ACH_TRAVEL_FAR_SINGLE = 3,
		};

#ifdef USE_STEAM
		// Achievement array which will hold data about the achievements and their state
		std::vector<Achievement> achievements =
		{
			_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
			_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
			_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
			_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
		};

		// Global access to Achievements object
		SteamAchievements* steamAchievements = nullptr;
#endif // USE_STEAM
#pragma endregion
	};
}

#endif // ACHIEVEMENTS_H