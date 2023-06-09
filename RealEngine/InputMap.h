#ifndef INPUTMAP_H
#define INPUTMAP_H

#include <map>
#include <stdexcept>
#include <SDL.h>

#include "Command.h"
#include "Input.h"

#define KEYPRESSED Uint32(-1)

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

		using ControllerKey = std::pair<int, XInputController::ControllerButton>;
		using CommandKeyRawPtr = std::pair<Command*, XInputController::InputType>;
		using ControllerCommandsMapRawPtr = std::map<ControllerKey, CommandKeyRawPtr>;
		ControllerCommandsMapRawPtr GetControllerCommands() const;

		using KeyboardKey = std::pair<Uint32 /*event*/, Uint32 /*scancode*/>;
		using KeyboardCommandsMapRawPtr = std::map<KeyboardKey, Command*>;
		KeyboardCommandsMapRawPtr GetKeyboardCommands() const;


		template <class T, typename... Args>
		void AddControllerCommands(XInputController::ControllerButton button, XInputController::InputType inputType, int controller, Args... commandArgs);
		template <class T, typename... Args>
		void AddKeyboardCommands(Uint32 scancode, Uint32 event, Args... commandArgs);
	private:
		std::string m_Name;

		using ControllerKey = std::pair<int, XInputController::ControllerButton>;
		using CommandKey = std::pair<std::unique_ptr<Command>, XInputController::InputType>;
		using ControllerCommandsMap = std::map<ControllerKey, CommandKey>;
		ControllerCommandsMap m_ControllerCommands{};

		using KeyboardKey = std::pair<Uint32 /*event*/, Uint32 /*scancode*/>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::unique_ptr<Command>>;
		KeyboardCommandsMap m_KeyboardCommands{};

		template <class T, typename... Args>
		void AddControllerCommandsHelper(int controllerIdx, XInputController::ControllerButton button, XInputController::InputType inputType, Args... commandArgs);
	};

	/**
	 * \brief
	 * \tparam T must derive from the Command class
	 * \param button
	 * \param inputType
	 * \param controller -1 will allow every controller to invoke this command
	 * \param commandArgs parameters for the Command class, must have a GameObject and optional params
	 *		for MoveCommand: FLOAT	speed (= 50)
	 */
	template <class T, typename... Args>
	inline void InputMap::AddControllerCommands(XInputController::ControllerButton button, XInputController::InputType inputType, int controller, Args... commandArgs)
	{
		static_assert(std::is_base_of<Command, T>(), "T must derive from the Command class");

		int controllerIdx{};
		for (const auto& pController : Input::GetInstance().GetControllers())
		{
			if (controller == -1)
			{

				//AddControllerCommandsHelper<T>(controllerIdx, button, inputType, commandArgs..., std::make_index_sequence<sizeof...(Args)>());
				m_ControllerCommands[std::pair(controllerIdx, button)] = std::pair(std::make_unique<T>(std::forward<Args>(commandArgs)...), inputType);
				m_ControllerCommands[std::pair(controllerIdx, button)] = std::pair<
					std::unique_ptr<T>, XInputController::InputType>(
					std::make_unique<T>(/*std::forward<Args>(*/commandArgs/*)*/...), inputType);

				++controllerIdx;
			}
			else if (pController->GetIndex() == controller)
			{
				m_ControllerCommands[std::pair(controller, button)] = std::pair(std::make_unique<T>(std::forward<Args>(commandArgs)...), inputType);
				return;
			}
		}

		if (controller != -1)
			throw std::runtime_error("controller doesn't exist");
	}

	/**
	 * \brief
	 * \tparam T must derive from the Command class
	 * \param scancode a SDL_SCANCODE_
	 * \param event SDL_KEYUP, SDL_KEYDOWN or KEYPRESSED, all other inputs will result in a runtime error
	 * \param commandArgs parameters for the Command class, must have a GameObject and optional params
	 *		for MoveCommand: FLOAT speed (= 50)
	 *		for DamageCommand: INT damage (= 1)
	 */
	template <class T, typename... Args>
	void InputMap::AddKeyboardCommands(Uint32 scancode, Uint32 event, Args... commandArgs)
	{
		if (event == SDL_KEYUP || event == SDL_KEYDOWN || event == KEYPRESSED)
			m_KeyboardCommands[std::pair(event, scancode)] = std::make_unique<T>(std::forward<Args>(commandArgs)...);
		else
			throw std::runtime_error("The event parameter must be either SDL_KEYDOWN or SDL_KEYUP");
	}

	template <class T, typename ... Args>
	void InputMap::AddControllerCommandsHelper(int controllerIdx, XInputController::ControllerButton button,
	                                           XInputController::InputType inputType, Args... commandArgs)
	{
		m_ControllerCommands[std::pair(controllerIdx, button)] = std::pair<std::unique_ptr<T>, XInputController::InputType>(std::make_unique<T>(std::move<Args>(commandArgs)...), inputType);
	}
}

#endif // INPUTMAP_H