#ifndef MUTECOMMAND_H
#define MUTECOMMAND_H

#include <Command.h>

class MuteCommand final : public real::Command
{
public:
	explicit MuteCommand(real::GameObject* object);
	virtual ~MuteCommand() override = default;
	MuteCommand(const MuteCommand& other) = delete;
	MuteCommand operator=(const MuteCommand& rhs) = delete;
	MuteCommand(MuteCommand&& other) = delete;
	MuteCommand operator=(MuteCommand&& rhs) = delete;

	void Execute() override;

private:
	bool m_IsMuted{};
};

#endif // MUTECOMMAND_H