#ifndef INPUT_H 
#define INPUT_H

#include <memory>
#include <SDL_events.h>
#include <string>
#include <vector>

#include "Singleton.h"
#include "Subject.h"
#include "XInputController.h"

namespace real
{
	class Command;
	class MoveCommand;
	class InputMap;

	class Input final : public Singleton<Input>
	{
	public:
		~Input() override;
		Input(const Input& other) = delete;
		Input operator=(const Input& rhs) = delete;
		Input(Input&& other) = delete;
		Input operator=(Input&& rhs) = delete;

		void ReloadCommands();
		void Update();

		void EnableCoOp(const bool enable) { m_EnableCoOp = enable; }
		bool IsCoOpEnabled() const { return m_EnableCoOp; }
		void UseKeyboard(const bool useKeyboard) { m_UseKeyboard = useKeyboard; }

		bool ProcessInput();
			
		const int AddController();
		const std::vector<int> AddControllers();
		XInputController* GetController(unsigned int idx) const;
		std::vector<XInputController*> GetControllers() const;

		InputMap* AddInputMap(const std::string& name);
		InputMap* GetInputMap(const std::string& name) const;
		void SetInputMapActive(const std::string& name);

		void StartReadingKeyboard() { m_IsReading = true; }
		void StopReadingKeyboard() { m_IsReading = false; }

		Subject<char> inputReceived{};

	private:
		friend class Singleton<Input>;
		explicit Input() = default;

		std::vector<std::unique_ptr<XInputController>> m_ControllerPtrs{};
		std::vector<InputMap*> m_InputMapPtrs{};
		InputMap* m_pActiveInputMap{};

		bool m_EnableCoOp{ false };
		bool m_UseKeyboard{ false };

		bool m_IsReading{};
		bool ReadKeyboardInput(SDL_Event e);
	};
}

#endif // INPUT_H