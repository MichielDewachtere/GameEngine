#ifndef LOADNEXTSCENECOMMAND_H
#define LOADNEXTSCENECOMMAND_H

#include <string>

#include <Command.h>

class LoadNextLevelCommand final : public real::Command
{
public:
	LoadNextLevelCommand(real::GameObject* object, std::string name);
	virtual ~LoadNextLevelCommand() override = default;
	LoadNextLevelCommand(const LoadNextLevelCommand& other) = delete;
	LoadNextLevelCommand operator=(const LoadNextLevelCommand& rhs) = delete;
	LoadNextLevelCommand(LoadNextLevelCommand&& other) = delete;
	LoadNextLevelCommand operator=(LoadNextLevelCommand&& rhs) = delete;

	void Execute() override;

private:
	std::string m_Name{ "empty" };
};

#endif // LOADNEXTSCENECOMMAND_H