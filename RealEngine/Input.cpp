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

void real::Input::Init()
{
	//using ControllerKey = std::pair<unsigned, XInputController::ControllerButton>;
	//using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
	//
	//m_ControllerPtrs.push_back(std::make_unique<XInputController>(0));
	//
	//ControllerKey controllerPair = std::make_pair(0, XInputController::ControllerButton::ButtonDown);
	//m_ControllerCommands.insert({ controllerPair, std::make_unique<TestCommand>(new GameObject()) });
}

void real::Input::ReloadCommands()
{
	for (const auto& inputMap : m_InputMapPtrs)
	{
		for (const auto& [first, command] : inputMap->GetControllerCommands())
		{
			command.first->Start();
		}

		for (const auto& [first, command] : inputMap->GetKeyboardCommands())
		{
			command->Start();
		}
	}
}

real::Command* real::Input::HandleInput()
{
	//for (const auto& pController : m_ControllerPtrs)
	//{
	//	for (const auto& [key, command] : m_ControllerCommands)
	//	{
	//		if (pController->IsPressed(key.second))
	//		{
	//			command.get()->Execute(pController->GetIndex());
	//			return command.get();
	//		}
	//	}
	//}

	return nullptr;
}

bool real::Input::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		if (m_pActiveInputMap == nullptr)
			return true;

		if (m_UseKeyboard)
		{
			for (const auto& [info, command] : m_pActiveInputMap->GetKeyboardCommands())
			{
				if (e.type == info.first && (info.first == SDL_KEYUP || info.first == SDL_KEYDOWN))
				{
					if (e.key.keysym.scancode == (int)info.second)
					{
						command->SetKeyBoardInput(static_cast<int>(info.second));
						command->SetInputController(nullptr);
						command->Execute();
					}
				}
			}
		}

#ifdef USE_IMGUI
		//process event for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif // USE_IMGUI
	}

	if (m_pActiveInputMap == nullptr)
		return true;

	for (const auto& [info, command] : m_pActiveInputMap->GetKeyboardCommands())
	{
		if (info.first != (Uint32)-1)
			continue;
		
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
		if (pKeyboardState[info.second])
		{
			command->SetKeyBoardInput(static_cast<int>(info.second));
			command->SetInputController(nullptr);
			command->Execute();
			//if (const auto moveCommand = dynamic_cast<MoveCommand*>(command))
			//{
			//	SetDirectionKeyboard(info.second, moveCommand);
			//}
		}
	}

	for (const auto& pController : m_ControllerPtrs)
	{
		for (const auto& [key, command] : m_pActiveInputMap->GetControllerCommands())
		{
			switch (command.second)
			{
			case XInputController::InputType::pressed:
			{
				if (pController->IsPressed(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->SetInputController(pController.get());
					command.first->SetKeyBoardInput(-1);
					command.first->Execute();
				}
				
				break;
			}
			case XInputController::InputType::down:
			{
				if (pController->IsDown(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->SetInputController(pController.get());
					command.first->SetKeyBoardInput(-1);
					command.first->Execute();
				}
				break;
			}
			case XInputController::InputType::up:
			{
				if (pController->IsUp(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->SetInputController(pController.get());
					command.first->SetKeyBoardInput(-1);
					command.first->Execute();
				}
				
				break;
			}
			case XInputController::InputType::leftThumbMoved:
			{
				if (pController->HasLeftThumbStickMoved() && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->SetInputController(pController.get());
					command.first->SetKeyBoardInput(-1);
					command.first->Execute();
					//if (const auto moveCommand = dynamic_cast<MoveCommand*>(command.first))
					//	moveCommand->SetDirection(pController->GetNormalizedLeftThumbStickPos());
				}
				break;
			}
			case XInputController::InputType::rightThumbMoved:
			{
				if (pController->HasRightThumbStickMoved() && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->SetInputController(pController.get());
					command.first->SetKeyBoardInput(-1);
					command.first->Execute();
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
	//auto pInputMap = std::make_unique<InputMap>(name);
	auto pInputMap = new InputMap(name);

	//const auto pInputMapPtr = pInputMap.get();
	const auto pInputMapPtr = pInputMap;

	//m_InputMapPtrs.emplace_back(std::move(pInputMap));
	m_InputMapPtrs.emplace_back(pInputMap);

	if (m_pActiveInputMap == nullptr)
	{
		//m_pActiveInputMap = m_InputMapPtrs.begin()->get();
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
			//m_pActiveInputMap = pInputMap.get();
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
			//m_pActiveInputMap = pInputMap.get();
			m_pActiveInputMap = pInputMap;
			return;
		}
	}

	Logger::LogWarning("InputManager => No input map found with the name {}", name);
}

void real::Input::Update()
{
	for (const auto& pController : m_ControllerPtrs)
	{
		pController->Update();
	}
}

const int real::Input::AddController()
{
	int controllerIdx = 0/*(int)m_ControllerPtrs.size() + 2*/;

	for (int i{ 1 }; i < 5; ++i)
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
			//throw std::runtime_error("No Controller found with index " + std::to_string(controllerIdx));
			controllerIdx = i;
		}
	}

	//if (controllerIdx >= 4)
	//	throw std::runtime_error("there can be a maximum of 4 controllers connected");

	m_ControllerPtrs.push_back(std::make_unique<XInputController>(controllerIdx));

	return controllerIdx;
}

const std::vector<int> real::Input::AddControllers()
{
	std::vector<int> controllers{};

	for (int i{ 0 }; i < 5; ++i)
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