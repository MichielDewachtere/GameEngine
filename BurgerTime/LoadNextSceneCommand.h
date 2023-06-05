#ifndef LOADNEXTSCENECOMMAND_H
#define LOADNEXTSCENECOMMAND_H

#include <string>

#include <Command.h>

class LoadNextSceneCommand final : public real::Command
{
public:
	LoadNextSceneCommand(real::GameObject* object, std::string name);
	virtual ~LoadNextSceneCommand() override = default;

	void Execute() override;

private:
	std::string m_Name{ "empty" };
};

#endif // LOADNEXTSCENECOMMAND_H