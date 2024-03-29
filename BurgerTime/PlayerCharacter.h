#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include <glm/vec2.hpp>

#include <Component.h>
#include <Observer.h>
#include <Subject.h>
#include <SceneManager.h>

class PlayerCharacter : public real::Component,
	public real::Observer<bool>,
	public real::Observer<real::Scene&, real::SceneManager::SceneSwitchState>
{
public:
	enum Stats
	{
		pepper = 1,
		health = 2
	};

	explicit PlayerCharacter(real::GameObject* pOwner);
	virtual ~PlayerCharacter() override = default;
	PlayerCharacter(const PlayerCharacter& other) = delete;
	PlayerCharacter& operator=(const PlayerCharacter& rhs) = delete;
	PlayerCharacter(PlayerCharacter&& other) = delete;
	PlayerCharacter& operator=(PlayerCharacter&& rhs) = delete;

	void Start() override;
	void Update() override;
	bool CanRender() const override { return false; }

	void HandleEvent(bool) override;
	//void HandleEvent(int, int) override;
	void HandleEvent(real::Scene&, real::SceneManager::SceneSwitchState) override;
	void OnSubjectDestroy() override {}

	void ThrowPepper();
	real::Subject<bool> pepperThrown{};
	real::Subject<int> amountOfPepperChanged{};
	real::Subject<int> pickedUpItem{};

	void SetDirection(glm::vec2 direction);


private:
	bool m_ItemSpawned{};
	bool m_PepperThrown{};
	static inline int m_Peppers{ 5 };
	float m_AccuTime{};
	float m_MaxStunTime{ 1.f };

	glm::vec2 m_CurrentDirection{};
	bool m_ReceivedUpdate{};

	real::GameObject* m_pItem{};
};

#endif // PLAYERCHARACTER_H