//#include "stdafx.h"
#include "SteamAchievements.h"

#ifdef USE_STEAM

real::SteamAchievements::SteamAchievements(Achievement* achievements, int numAchievements) 
	: m_CallbackUserStatsReceived(this, &SteamAchievements::OnUserStatsReceived)
    , m_CallbackUserStatsStored(this, &SteamAchievements::OnUserStatsStored)
    , m_CallbackAchievementStored(this, &SteamAchievements::OnAchievementStored)
    , m_AppId(0)
    , m_Initialized(false)
{
    m_AppId = SteamUtils()->GetAppID();
    m_pAchievements = achievements;
    m_NumAchievements = numAchievements;
    RequestStats();
}

real::SteamAchievements::~SteamAchievements() = default;

bool real::SteamAchievements::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool real::SteamAchievements::SetAchievement(const char* ID)
{
	// Have we received a call back from Steam yet?
	if (m_Initialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void real::SteamAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_AppId == (int64)pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString("Received stats and achievements from Steam\n");
			m_Initialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_NumAchievements; ++iAch)
			{
				Achievement& ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.pAchievementId, &ach.achieved);
				_snprintf_s(ach.name, sizeof(ach.name), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.pAchievementId,
						"name"));
				_snprintf_s(ach.description, sizeof(ach.description), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.pAchievementId,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}

void real::SteamAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_AppId == (int64)pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString("Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}

void real::SteamAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_AppId == (int64)pCallback->m_nGameID)
	{
		OutputDebugString("Stored Achievement for Steam\n");
	}
}

#endif // USE_STEAM