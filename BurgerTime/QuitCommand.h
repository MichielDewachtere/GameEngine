#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include <Command.h>

class QuitCommand final : public real::Command
{
public:
	explicit QuitCommand(real::GameObject* object);
	virtual ~QuitCommand() override = default;
	QuitCommand(const QuitCommand& other) = delete;
	QuitCommand operator=(const QuitCommand& rhs) = delete;
	QuitCommand(QuitCommand&& other) = delete;
	QuitCommand operator=(QuitCommand&& rhs) = delete;

	void Execute() override;
};

#endif // QUITCOMMAND_H