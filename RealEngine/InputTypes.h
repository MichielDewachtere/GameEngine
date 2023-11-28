#ifndef INPUTTYPES_H
#define INPUTTYPES_H

#include <memory>
#include "Command.h"
#include "XInputController.h"

namespace real
{
	struct ComputerInput
	{
		uint32_t event, scancode;
		std::unique_ptr<Command> command;

		ComputerInput(uint32_t event, uint32_t scancode, Command* command)
			: event(event), scancode(scancode), command(command) {}

	};

	struct ControllerInput
	{
		uint8_t controller;
		XInputController::ControllerButton button;
		XInputController::InputType inputType;
		std::unique_ptr<Command> command;

		ControllerInput(uint8_t controller, XInputController::ControllerButton button, XInputController::InputType inputType, Command* command)
			: controller(controller), button(button), inputType(inputType), command(command) {}
	};
}

#endif // INPUTTYPES_H