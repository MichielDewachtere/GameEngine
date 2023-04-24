#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include "Component.h"

namespace real
{
    class PlayerCharacter : public Component
    {
    public:
	    explicit PlayerCharacter(GameObject* pOwner);
		virtual ~PlayerCharacter() = default;
		PlayerCharacter(const PlayerCharacter& other) = delete;
		PlayerCharacter& operator=(const PlayerCharacter& rhs) = delete;
		PlayerCharacter(PlayerCharacter&& other) = delete;
		PlayerCharacter& operator=(PlayerCharacter&& rhs) = delete;

    private:
		//void PickUp
    };
}

#endif // PLAYERCHARACTER_H