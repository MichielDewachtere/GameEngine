#ifndef INPUT_H 
#define INPUT_H

#include <memory>
#include <SDL_stdinc.h>
#include <string>
#include <vector>

#include "Singleton.h"
#include "XInputController.h"
//#include "InputMap.h"

namespace real
{
	class Command;
	class MoveCommand;
	class InputMap;

	class Input final : public Singleton<Input>
	{
	public:
		~Input() override;

		void Init();

		void Update();

		void EnableCoOp(const bool enable) { m_EnableCoOp = enable; }
		bool IsCoOpEnabled() const { return m_EnableCoOp; }
		void UseKeyboard(const bool useKeyboard) { m_UseKeyboard = useKeyboard; }

		Command* HandleInput();
		bool ProcessInput();
			
		const int AddController();
		const std::vector<int> AddControllers();
		XInputController* GetController(unsigned int idx) const;
		std::vector<XInputController*> GetControllers() const;

		InputMap* AddInputMap(const std::string& name);
		void SetInputMapActive(const std::string& name);

	private:
		std::vector<std::unique_ptr<XInputController>> m_ControllerPtrs{};
		//std::vector<std::unique_ptr<InputMap>> m_InputMapPtrs{};
		std::vector<InputMap*> m_InputMapPtrs{};
		InputMap* m_pActiveInputMap{};

		bool m_EnableCoOp{ false };
		bool m_UseKeyboard{ false };

		void SetDirectionKeyboard(Uint32 scancode, MoveCommand* command);
	};
}

#endif // INPUT_H