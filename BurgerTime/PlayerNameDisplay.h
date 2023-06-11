#ifndef PLAYERDISPLAYNAME_H
#define PLAYERDISPLAYNAME_H

#include <Component.h>
#include <Observer.h>

class PlayerNameDisplay final : public real::Component,
	public real::Observer<char>
{
public:
	explicit PlayerNameDisplay(real::GameObject* pOwner);
	virtual ~PlayerNameDisplay() override;
	PlayerNameDisplay(const PlayerNameDisplay& other) = delete;
	PlayerNameDisplay operator=(const PlayerNameDisplay& rhs) = delete;
	PlayerNameDisplay(PlayerNameDisplay&& other) = delete;
	PlayerNameDisplay operator=(PlayerNameDisplay&& rhs) = delete;

	void Start() override;
	void Update() override {}
	bool CanRender() const override { return false; }
	void HandleEvent(char) override;
	void OnSubjectDestroy() override {}

private:
	std::string m_PlayerName{};
};

#endif // PLAYERDISPLAYNAME_H