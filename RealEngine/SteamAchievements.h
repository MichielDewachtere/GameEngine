#ifndef STEAMACHIEVEMENTS_H
#define STEAMACHIEVEMENTS_H

#include "stdafx.h"

#ifdef USE_STEAM


#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
namespace real
{
	struct Achievement
	{
		int achievementId;
		const char* pAchievementId;
		char name[128];
		char description[256];
		bool achieved;
		int iconImage;
	};

	class SteamAchievements final //TODO: make this a singleton?
	{
	public:
		SteamAchievements(Achievement* achievements, int numAchievements);
		~SteamAchievements();
		SteamAchievements(const SteamAchievements& other) = delete;
		SteamAchievements& operator=(const SteamAchievements& rhs) = delete;
		SteamAchievements(SteamAchievements&& other) = delete;
		SteamAchievements& operator=(SteamAchievements&& rhs) = delete;

		bool RequestStats();
		bool SetAchievement(const char* ID);

		STEAM_CALLBACK(SteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);

	private:
		int64 m_AppId; // Our current AppID
		Achievement* m_pAchievements; // Achievements data
		int m_NumAchievements; // The number of Achievements
		bool m_Initialized; // Have we called Request stats and received the callback?
	};
}

#endif // USE_STEAM

#endif // STEAMACHIEVEMENTS_H