#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <Observer.h>
#include <Singleton.h>
#include <Scene.h>

class PlayerManager final
	: public real::Singleton<PlayerManager>
	, public real::Observer<real::Scene&>
{
public:
	~PlayerManager() override;
	PlayerManager(const PlayerManager& other) = delete;
	PlayerManager operator=(const PlayerManager& rhs) = delete;
	PlayerManager(PlayerManager&& other) = delete;
	PlayerManager operator=(PlayerManager&& rhs) = delete;

	void Init();
	
	void HandleEvent(real::Scene&) override;
	void OnSubjectDestroy() override {}

	void AddPlayer(bool useKeyboard = true, const int controllerIdx = -1);
	int GetAmountOfPlayers() const { return static_cast<int>(m_PlayerPtrs.size()); }

private:
	friend class Singleton<PlayerManager>;
	PlayerManager() = default;

	std::vector<std::shared_ptr<real::GameObject>> m_PlayerPtrs{};

	bool m_KeyboardInUse{};
	std::vector<int> m_ControllersInUse{};

	std::shared_ptr<real::GameObject> m_pHud{};
	void InitHud();
};

#endif // PLAYERMANAGER_H