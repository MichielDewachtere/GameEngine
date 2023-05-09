#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include <Component.h>

class PlayerCharacter : public real::Component
{
public:
	explicit PlayerCharacter(real::GameObject* pOwner);
	virtual ~PlayerCharacter() = default;
	PlayerCharacter(const PlayerCharacter& other) = delete;
	PlayerCharacter& operator=(const PlayerCharacter& rhs) = delete;
	PlayerCharacter(PlayerCharacter&& other) = delete;
	PlayerCharacter& operator=(PlayerCharacter&& rhs) = delete;

private:
	//void PickUp
};

#endif // PLAYERCHARACTER_H