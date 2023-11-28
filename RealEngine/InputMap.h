#ifndef INPUTMAP_H
#define INPUTMAP_H

#include <map>
#include <stdexcept>
#include <SDL.h>

#include "Input.h"
//#include "Command.h"
#include "InputTypes.h"

#define KEYPRESSED Uint32(0)

namespace real
{
	class InputMap final
	{
	public:
		explicit InputMap(std::string name) : m_Name(std::move(name)) {}
		~InputMap() = default;
		InputMap(const InputMap& other) = delete;
		InputMap& operator=(const InputMap& rhs) = delete;
		InputMap(InputMap&& other) = delete;
		InputMap& operator=(InputMap&& rhs) = delete;

		std::string GetName() const { return m_Name; }

		std::vector<ComputerInput*> GetComputerInputs() const;
		Command& GetComputerCommand(uint8_t id) const;
		std::vector<ControllerInput*> GetControllerInputs() const;
		Command& GetControllerCommand(uint8_t id) const;

		template <class T, typename... Args>
		void AddMouseInput(uint8_t id, uint32_t event, uint32_t button, Args... commandArgs);
		template <class T, typename... Args>
		void AddKeyboardInput(uint8_t id, uint32_t event, uint32_t scancode, Args... commandArgs);
		template <class T, typename... Args>
		void AddControllerInput(uint8_t id, XInputController::ControllerButton button, XInputController::InputType inputType, Args... commandArgs);
		template <class T, typename... Args>
		void AddControllerInput(uint8_t id, uint8_t controller, XInputController::ControllerButton button, XInputController::InputType inputType, Args... commandArgs);

	private:
		std::string m_Name;

		std::map<uint8_t /*id*/, std::unique_ptr<ComputerInput>> m_ComputerInputs{};
		std::map<uint8_t /*id*/, std::unique_ptr<ControllerInput>> m_ControllerInputs{};

		template <class T, typename... Args>
		void AddComputerInput(uint8_t id, uint32_t event, uint32_t scancode, Args... commandArgs);

		template <typename T>
		std::vector<T*> GetInputs(const std::map<uint8_t, std::unique_ptr<T>>& inputs) const;
	};

	template <class T, typename ... Args>
	void InputMap::AddMouseInput(uint8_t id, uint32_t event, uint32_t button, Args... commandArgs)
	{
		static_assert(std::is_base_of<Command, T>(), "T must derive from the Command class");

		if (button != SDL_BUTTON_LEFT && button != SDL_BUTTON_RIGHT)
			throw std::runtime_error("The scancode parameter must be either SDL_SCANCODE_LEFT or SDL_SCANCODE_RIGHT");

		if (event != SDL_MOUSEBUTTONUP && event != SDL_MOUSEBUTTONDOWN)
			throw std::runtime_error("The event parameter must be either SDL_MOUSEBUTTONUP or SDL_MOUSEBUTTONDOWN");

		if (m_ComputerInputs.contains(id))
			throw std::runtime_error("A mouse input is already registered with id: " + id);

		m_ComputerInputs.try_emplace(id, std::make_unique<ComputerInput>(event, button, new T(commandArgs...)));
	}

	template <class T, typename ... Args>
	void InputMap::AddKeyboardInput(uint8_t id, uint32_t event, uint32_t scancode, Args... commandArgs)
	{
		static_assert(std::is_base_of<Command, T>(), "T must derive from the Command class");

		if (event != SDL_KEYUP && event != SDL_KEYDOWN && event != KEYPRESSED)
			throw std::runtime_error("The scancode parameter must be either SDL_KEYUP, SDL_KEYDOWN or KEYPRESSED");

		if (m_ComputerInputs.contains(id))
			throw std::runtime_error("A mouse input is already registered with id: " + id);

		m_ComputerInputs.try_emplace(id, std::make_unique<ComputerInput>(event, scancode, new T(commandArgs...)));
	}

	template <class T, typename ... Args>
	void InputMap::AddControllerInput(uint8_t id, XInputController::ControllerButton button, XInputController::InputType inputType, Args... commandArgs)
	{
		for (size_t i = 0; i < Input::GetInstance().GetControllers().size(); ++i)
		{
			AddControllerInput<T>(id, static_cast<uint8_t>(i), button, inputType, commandArgs...);
		}
	}

	template <class T, typename ... Args>
	void InputMap::AddControllerInput(uint8_t id, uint8_t controller, XInputController::ControllerButton button, XInputController::InputType inputType,
		Args... commandArgs)
	{
		static_assert(std::is_base_of<Command, T>(), "T must derive from the Command class");

		if (m_ControllerInputs.contains(id))
			throw std::runtime_error("A controller input is already registered with id: " + id);

		m_ControllerInputs.try_emplace(id, std::make_unique<ControllerInput>(controller, button, inputType, new T(commandArgs...)));
	}

	template <class T, typename ... Args>
	void InputMap::AddComputerInput(uint8_t id, uint32_t event, uint32_t scancode, Args... commandArgs)
	{
		m_ComputerInputs.try_emplace(id, std::make_unique<ComputerInput>(event, scancode, new T(commandArgs...)));
	}

	template <typename T>
	std::vector<T*> InputMap::GetInputs(const std::map<uint8_t, std::unique_ptr<T>>& inputs) const
	{
		std::vector<T*> v;

		for (const auto& input : inputs)
		{
			v.push_back(input.second.get());
		}

		return v;
	}
}

#endif // INPUTMAP_H