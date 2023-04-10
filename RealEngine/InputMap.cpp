#include "stdafx.h"
#include "InputMap.h"

real::InputMap::ControllerCommandsMapRawPtr real::InputMap::GetControllerCommands() const
{
	using ControllerKey = std::pair<unsigned, XInputController::ControllerButton>;
	using CommandKeyRawPtr = std::pair<Command*, XInputController::InputType>;
	using ControllerCommandsMapRawPtr = std::map<ControllerKey, CommandKeyRawPtr>;

	ControllerCommandsMapRawPtr map{};

	for (const auto& [controllerKey, commandKey] : m_ControllerCommands)
	{
		auto first = std::pair{ controllerKey.first, controllerKey.second };
		auto second = std::pair{ commandKey.first.get(), commandKey.second };

		map.insert(std::pair{ first, second });
	}

	return map;
}

real::InputMap::KeyboardCommandsMapRawPtr real::InputMap::GetKeyboardCommands() const
{
	using KeyboardKey = std::pair<Uint32 /*event*/, Uint32 /*scancode*/>;
	using KeyboardCommandsMapRawPtr = std::map<KeyboardKey, Command*>;

	KeyboardCommandsMapRawPtr map{};

	for (const auto& commands : m_KeyboardCommands)
	{
		auto first = commands.first;
		auto second = commands.second.get();

		map.insert(std::pair{ first, second });
	}

	return map;
}
