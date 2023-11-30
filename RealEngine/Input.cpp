//#include "stdafx.h"
#include "Input.h"

#include <SDL_syswm.h>

#include <Xinput.h>
#include <glm/glm.hpp>

#include "InputMap.h"
#include "Logger.h"

real::Input::~Input()
{
	for (const auto pInputMap : m_InputMapPtrs)
		delete pInputMap;
}

void real::Input::ReloadCommands() const
{
	for (const auto& inputMap : m_InputMapPtrs)
	{
		for (const auto& input : inputMap->GetComputerInputs())
		{
			input->command->Start();
		}

		for (const auto& input : inputMap->GetControllerInputs())
		{
			input->command->Start();
		}
	}
}

bool real::Input::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			return false;

		if (m_pActiveInputMap == nullptr)
			return true;

		// Read keyboard
		if (m_IsReading)
		{
			if (ReadKeyboardInput(e))
				return true;
		}

		// Execute keyboard & mouse
		if (m_UseKeyboard == false)
			break;

		for (const auto& input : m_pActiveInputMap->GetComputerInputs())
		{
			if (e.type == input->event)
			{
				if (e.key.keysym.scancode == static_cast<SDL_Scancode>(input->scancode)
				   || e.button.button == static_cast<Uint8>(input->scancode))
				{
					ExecuteCommand(input->command.get(), nullptr, static_cast<int>(input->scancode));
				}
			}
			//else if (input->event == KEYPRESSED)
			//{
			//	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
			//	if (pKeyboardState[input->scancode])
			//	{
			//		ExecuteCommand(input->command.get(), nullptr, static_cast<int>(input->scancode));
			//	}
			//}
		}
#ifdef USE_IMGUI
		//process event for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif // USE_IMGUI
	}

	if (m_UseKeyboard)
	{
		for (const auto& input : m_pActiveInputMap->GetComputerInputs())
		{
			if (input->event == KEYPRESSED)
			{
				const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
				if (pKeyboardState[input->scancode])
				{
					ExecuteCommand(input->command.get(), nullptr, static_cast<int>(input->scancode));
				}
			}
		}
	}

	// Controller Logic
	for (const auto& pController : m_ControllerPtrs)
	{
		for (const auto& input : m_pActiveInputMap->GetControllerInputs())
		{
			if (input->controller != pController->GetIndex())
				continue;

			switch (input->inputType)
			{
			case XInputController::InputType::pressed:
			{
				if (pController->IsPressed(input->button))
				{
					ExecuteCommand(input->command.get(), pController.get(), -1);
				}
				
				break;
			}
			case XInputController::InputType::down:
			{
				if (pController->IsDown(input->button))
				{
					ExecuteCommand(input->command.get(), pController.get(), -1);
				}
				break;
			}
			case XInputController::InputType::up:
			{
				if (pController->IsUp(input->button))
				{
					ExecuteCommand(input->command.get(), pController.get(), -1);
				}
				
				break;
			}
			case XInputController::InputType::leftThumbMoved:
			{
				if (pController->HasLeftThumbStickMoved())
				{
					ExecuteCommand(input->command.get(), pController.get(), -1);
				}
				break;
			}
			case XInputController::InputType::rightThumbMoved:
			{
				if (pController->HasRightThumbStickMoved())
				{
					ExecuteCommand(input->command.get(), pController.get(), -1);
				}
				break;
			}
			}
		}
	}

	return true;
}

real::XInputController* real::Input::GetController(const unsigned idx) const
{
	for (const auto& pController : m_ControllerPtrs)
	{
		if (pController->GetIndex() == static_cast<int>(idx))
			return pController.get();
	}
	return nullptr;
}

std::vector<real::XInputController*> real::Input::GetControllers() const 
{
	std::vector<XInputController*> controllerPtrs;

	for (const auto& pController : m_ControllerPtrs)
		controllerPtrs.push_back(pController.get());

	return controllerPtrs;
}

real::InputMap* real::Input::AddInputMap(const std::string& name)
{
	auto pInputMap = new InputMap(name);
	const auto pInputMapPtr = pInputMap;

	m_InputMapPtrs.emplace_back(pInputMap);

	if (m_pActiveInputMap == nullptr)
	{
		m_pActiveInputMap = m_InputMapPtrs[0];
	}

	return pInputMapPtr;
}

real::InputMap* real::Input::GetInputMap(const std::string& name) const
{
	for (const auto& pInputMap : m_InputMapPtrs)
	{
		if (pInputMap->GetName() == name)
		{
			return pInputMap;
		}
	}

	Logger::LogWarning("InputManager => No input map found with the name {}", name);
	return nullptr;
}

void real::Input::SetInputMapActive(const std::string& name)
{
   	for (const auto& pInputMap : m_InputMapPtrs)
	{
		if (pInputMap->GetName() == name)
		{
			m_pActiveInputMap = pInputMap;
			return;
		}
	}

	Logger::LogWarning("InputManager => No input map found with the name {}", name);
}

glm::vec2 real::Input::GetMousePosition()
{
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	return { mouseX, mouseY };
}

bool real::Input::ReadKeyboardInput(SDL_Event e)
{
	if (e.type == SDL_KEYUP)
	{
		if (e.key.keysym.scancode >= SDL_SCANCODE_A 
			&& e.key.keysym.scancode <= SDL_SCANCODE_Z)
		{
				constexpr int toAscii = 61;
				inputReceived.Notify(static_cast<char>(e.key.keysym.scancode + toAscii));
				return true;
		}
	}

	return false;
}

void real::Input::ExecuteCommand(Command* command, XInputController* pController, int button)
{
	command->Execute();
	command->SetInputController(pController);
	command->SetKeyBoardInput(button);
}

void real::Input::Update() const
{
	for (const auto& pController : m_ControllerPtrs)
	{
		pController->Update();
	}
}

uint8_t real::Input::AddController()
{
	uint8_t controllerIdx = 0;

	for (uint8_t i{ 1 }; i < 5; ++i)
	{
		bool indexExists = false;
		for (const auto& pController : m_ControllerPtrs)
		{
			if (i == pController->GetIndex())
				indexExists = true;
			else
				indexExists = false;
		}

		if (indexExists == true)
			continue;

		XINPUT_STATE state;
		const DWORD result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			controllerIdx = i;
		}
	}

	m_ControllerPtrs.push_back(std::make_unique<XInputController>(controllerIdx));

	return controllerIdx;
}

std::vector<uint8_t> real::Input::AddControllers()
{
	std::vector<uint8_t> controllers{};

	for (uint8_t i{ 0 }; i < 5; ++i)
	{
		XINPUT_STATE state;
		const DWORD result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			m_ControllerPtrs.push_back(std::make_unique<XInputController>(i));
			controllers.push_back(i);
		}
	}

	return controllers;
}