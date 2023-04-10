#include "stdafx.h"
#include "Input.h"

#include "InputMap.h"

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

		if (m_UseKeyboard)
		{
			for (const auto& [info, command] : m_pActiveInputMap->GetKeyboardCommands())
			{
				if (e.type == info.first && (info.first == SDL_KEYUP || info.first == SDL_KEYDOWN))
				{
					if (e.key.keysym.scancode == (int)info.second)
					{
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

	for (const auto& [info, command] : m_pActiveInputMap->GetKeyboardCommands())
	{
		if (info.first != (Uint32)-1)
			continue;
		
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
		if (pKeyboardState[info.second])
		{
			command->Execute();
			if (const auto moveCommand = dynamic_cast<MoveCommand*>(command))
			{
				SetDirectionKeyboard(info.second, moveCommand);
			}
		}
	}

	for (const auto& pController : m_ControllerPtrs)
	{
		for (const auto& [key, command] : m_pActiveInputMap->GetControllerCommands())
		{
			switch (command.second)
			{
			case XInputController::InputType::pressed:
				if (pController->IsPressed(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
					command.first->Execute();
				
				break;
			case XInputController::InputType::down:
				if (pController->IsDown(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
					command.first->Execute();
				break;
			case XInputController::InputType::up:
				if (pController->IsUp(key.second) && key.first == static_cast<unsigned int>(pController->GetIndex()))
					command.first->Execute();
				
				break;
			case XInputController::InputType::leftThumbMoved: 
				if (pController->HasLeftThumbStickMoved() && key.first == static_cast<unsigned int>(pController->GetIndex()))
				{
					command.first->Execute();
					if (const auto moveCommand = dynamic_cast<MoveCommand*>(command.first))
						moveCommand->SetDirection(pController->GetNormalizedLeftThumbStickPos());
				}
				break;
			case XInputController::InputType::rightThumbMoved: 
				if (pController->HasRightThumbStickMoved() && key.first == static_cast<unsigned int>(pController->GetIndex()))
					command.first->Execute();
				break;
			}
		}
	}

	//for (const auto& [key, command] : m_ControllerCommands)
	//{
	//	if (m_ControllerPtrs[key.first]->HasLeftThumbStickMoved(key.second))
	//	{
	//		command->Execute(key.first);
	//	}
	//}

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

void real::Input::SetInputMapActive(const std::string& name)
{
	for (const auto& pInputMap : m_InputMapPtrs)
	{
		if (pInputMap->GetName() == name)
		{
			//m_pActiveInputMap = pInputMap.get();
			m_pActiveInputMap = pInputMap;
		}
	}
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

	for (int i{ 1 }; i < 5; ++i)
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

void real::Input::SetDirectionKeyboard(Uint32 scancode, real::MoveCommand* command)
{
	glm::vec2 direction{};

	if (scancode == SDL_SCANCODE_A)
		direction = { -1,0 };
	else if (scancode == SDL_SCANCODE_D)
		direction = { 1,0 };
	else if (scancode == SDL_SCANCODE_W)
		direction = { 0,1 };
	else if (scancode == SDL_SCANCODE_S)
		direction = { 0,-1 };

	direction = normalize(direction);
	command->SetDirection(direction);
}
