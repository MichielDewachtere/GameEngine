#ifndef PLAYERJOINCOMMAND_H
#define PLAYERJOINCOMMAND_H

#include <Command.h>

class PlayerJoinCommand final : public real::Command
{
public:
	explicit PlayerJoinCommand(real::GameObject* object);
	virtual ~PlayerJoinCommand() override = default;
	PlayerJoinCommand(const PlayerJoinCommand& other) = delete;
	PlayerJoinCommand& operator=(const PlayerJoinCommand& other) = delete;
	PlayerJoinCommand(PlayerJoinCommand&& other) = delete;
	PlayerJoinCommand& operator=(PlayerJoinCommand&& other) = delete;

	void Execute() override;
};

#endif // PLAYERJOINCOMMAND_H