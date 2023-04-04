#ifndef INPUT_H 
#define INPUT_H

#include "Singleton.h"
#include "XInputController.h"
#include "Command.h"

#define KEYPRESSED Uint32(-1)

namespace real
{
	class Command;

	class Input final : public Singleton<Input>
	{
	public:
		void Init();

		void Update();

		void EnableCoOp(const bool enable) { m_EnableCoOp = enable; }
		bool IsCoOpEnabled() const { return m_EnableCoOp; }
		void UseKeyboard(const bool useKeyboard) { m_UseKeyboard = useKeyboard; }

		Command* HandleInput();
		bool ProcessInput();
		XInputController* GetController(unsigned int idx) const;
			
		const int AddController();
		const std::vector<int> AddControllers();
		template <class T, typename... Args>
		void AddControllerCommands(XInputController::ControllerButton button, XInputController::InputType inputType, unsigned int controller, Args... commandArgs);
		template <class T, typename... Args>
		void AddKeyboardCommands(Uint32 scancode, Uint32 event, Args... commandArgs);

	private:
		using ControllerKey = std::pair<unsigned, XInputController::ControllerButton>;
		using CommandKey = std::pair<std::unique_ptr<Command>, XInputController::InputType>;
		using ControllerCommandsMap = std::map<ControllerKey, CommandKey>;

		ControllerCommandsMap m_ConsoleCommands{};
		std::vector<std::unique_ptr<XInputController>> m_ControllerPtrs{};

		using KeyboardKey = std::pair<Uint32 /*event*/, Uint32 /*scancode*/>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::unique_ptr<Command>>;
		KeyboardCommandsMap m_KeyboardCommands{};

		bool m_EnableCoOp{ false };
		bool m_UseKeyboard{ false };

		void SetDirectionKeyboard(Uint32 scancode, MoveCommand* command);
	};

	/**
	 * \brief 
	 * \tparam T must derive from the Command class
	 * \param button 
	 * \param inputType 
	 * \param controller
	 * \param commandArgs parameters for the Command class, must have a GameObject and optional params
	 *		for MoveCommand: FLOAT	speed (= 50)
	 */
	template <class T, typename... Args>
	inline void Input::AddControllerCommands(XInputController::ControllerButton button, XInputController::InputType inputType, unsigned int controller, Args... commandArgs)
	{
		static_assert(std::is_base_of<Command, T>(), "T must derive from the Command class");

		for (const auto& pController : m_ControllerPtrs)
		{
			if (pController.get()->GetIndex() == (int)controller)
			{
				m_ConsoleCommands[std::pair(controller, button)] = std::pair(std::make_unique<T>(std::forward<Args>(commandArgs)...), inputType);
				return;
			}
		}

		throw std::runtime_error("controller doesn't exist");
	}

	/**
	 * \brief 
	 * \tparam T must derive from the Command class
	 * \param go 
	 * \param scancode a SDL_SCANCODE_
	 * \param event SDL_KEYUP, SDL_KEYDOWN or KEYPRESSED, all other inputs will result in a runtime error
	 * \param commandArgs parameters for the Command class, must have a GameObject and optional params
	 *		for MoveCommand: FLOAT speed (= 50)
	 *		for DamageCommand: INT damage (= 1)
	 */
	template <class T, typename... Args>
	void Input::AddKeyboardCommands(Uint32 scancode, Uint32 event, Args... commandArgs)
	{
		if (event == SDL_KEYUP || event == SDL_KEYDOWN || event == KEYPRESSED)
			m_KeyboardCommands[std::pair(event, scancode)] = std::make_unique<T>(std::forward<Args>(commandArgs)...);
		else
			throw std::runtime_error("The event parameter must be either SDL_KEYDOWN or SDL_KEYUP");
	}
}

#endif // INPUT_H