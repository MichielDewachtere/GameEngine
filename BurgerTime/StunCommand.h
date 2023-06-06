#pragma once

#include <Command.h>

class StunCommand final : public real::Command
{
public:
	explicit StunCommand(real::GameObject* object);
	virtual ~StunCommand() override = default;
	StunCommand(const StunCommand& other) = delete;
	StunCommand operator=(const StunCommand& rhs) = delete;
	StunCommand(StunCommand&& other) = delete;
	StunCommand operator=(StunCommand&& rhs) = delete;

	virtual void Execute() override;
};
