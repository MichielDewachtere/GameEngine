//#include "stdafx.h"
#include "InputMap.h"

#include <ranges>

std::vector<real::ComputerInput*> real::InputMap::GetComputerInputs() const
{
	return GetInputs<ComputerInput>(m_ComputerInputs);
}

real::Command& real::InputMap::GetComputerCommand(uint8_t id) const
{
	if (m_ComputerInputs.contains(id) == false)
		throw std::runtime_error("Computer input with the given id: " + std::to_string(id) + ", not found");

	return *m_ComputerInputs.at(id)->command;
}

std::vector<real::ControllerInput*> real::InputMap::GetControllerInputs() const
{
	return GetInputs<ControllerInput>(m_ControllerInputs);
}

real::Command& real::InputMap::GetControllerCommand(uint8_t id) const
{
	if (m_ControllerInputs.contains(id) == false)
		throw std::runtime_error("Controller input with the given id: " + std::to_string(id) + ", not found");

	return *m_ControllerInputs.at(id)->command;
}