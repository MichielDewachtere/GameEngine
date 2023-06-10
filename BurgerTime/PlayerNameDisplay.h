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
	PlayerNameDisplay(const Component& other) = delete;
	PlayerNameDisplay operator=(const Component& rhs) = delete;
	PlayerNameDisplay(Component&& other) = delete;
	PlayerNameDisplay operator=(Component&& rhs) = delete;

	void Start() override;
	void Update() override {}
	bool CanRender() const override { return false; }
	void HandleEvent(char) override;
	void OnSubjectDestroy() override {}
};

#endif // PLAYERDISPLAYNAME_H