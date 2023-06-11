#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <Observer.h>
#include <Singleton.h>
#include <Scene.h>

#include "Ingredient.h"

class PlayerManager final
	: public real::Singleton<PlayerManager>
	, public real::Observer<real::Scene&>
	, public real::Observer<Ingredient&>
{
public:
	~PlayerManager() override;
	PlayerManager(const PlayerManager& other) = delete;
	PlayerManager operator=(const PlayerManager& rhs) = delete;
	PlayerManager(PlayerManager&& other) = delete;
	PlayerManager operator=(PlayerManager&& rhs) = delete;

	void Init();
	
	void HandleEvent(real::Scene&) override;
	void HandleEvent(Ingredient&) override;
	void OnSubjectDestroy() override {}

	void AddPlayer(bool useKeyboard = true, const int controllerIdx = -1);
	int GetAmountOfPlayers() const { return static_cast<int>(m_PlayerPtrs.size()); }
	//int GetPlayerIndex(const real::GameObject& player) const;
	real::GameObject* GetPlayer(int idx) const { return m_PlayerPtrs[idx].get(); }
	std::vector<real::GameObject*> GetPlayers() const;

	void SubmitName(std::string name);
	std::string GetPlayerName() const { return m_PlayerName; }
	void SetScore(int score) { m_Score = score; }
	int GetScore() const { return m_Score; }

	int GetCurrentLevel() const { return m_CurrentLevel; }
	int GetCurrentLoop() const { return m_CurrentLoop; }
	//void ResetLevel();

	real::GameObject* GetHUD() const { return m_pHud.get(); }

	real::Subject<> levelHasEnded{};
	real::Subject<int> playerJoined{};
	real::Subject<bool> playerStopMoving;

private:
	friend class Singleton<PlayerManager>;
	PlayerManager() = default;

	std::vector<std::shared_ptr<real::GameObject>> m_PlayerPtrs{};
	std::string m_PlayerName{};
	int m_Score{};

	bool m_KeyboardInUse{};
	std::vector<int> m_ControllersInUse{};

	std::shared_ptr<real::GameObject> m_pHud{};
	void InitHud();

	int m_AmountOfPlates{};
	float m_CelebrationTimer{ 3.f };
	float m_AccuTime{};

	int m_CurrentLevel{ 0 };
	int m_CurrentLoop{ 1 };
	bool m_GameHasBegun{};

	void PlayerWins();
};

#endif // PLAYERMANAGER_H