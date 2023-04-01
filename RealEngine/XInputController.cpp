#define WIN_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "XInputController.h"

#define MAX_THUMBSTICK_VALUE 32767
#define DEAD_ZONE 5000

using namespace real;

class XInputController::XInputControllerImpl
{
public:
	explicit XInputControllerImpl(const int controllerIdx)
		: m_ControllerIndex(controllerIdx)
	{
		ZeroMemory(&m_PreviousState, sizeof XINPUT_STATE);
		ZeroMemory(&m_CurrentState, sizeof XINPUT_STATE);
	}

	~XInputControllerImpl() = default;
	XInputControllerImpl(const XInputControllerImpl& other) = delete;
	XInputControllerImpl operator=(const XInputControllerImpl& rhs) = delete;
	XInputControllerImpl(XInputControllerImpl&& other) = delete;
	XInputControllerImpl operator=(XInputControllerImpl&& rhs) = delete;


	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof XINPUT_STATE);
		ZeroMemory(&m_CurrentState, sizeof XINPUT_STATE);
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	int GetIndex() const { return m_ControllerIndex; }

	glm::vec2 GetNormalizedLeftThumbStickPos() const
	{
		glm::vec2 pos;

		pos.x = m_CurrentState.Gamepad.sThumbLX;
		pos.y = m_CurrentState.Gamepad.sThumbLY;

		//check if the controller is outside a circular dead zone
		if (pos.x > DEAD_ZONE || pos.x < -DEAD_ZONE)
		{
			//clip the magnitude at its expected maximum value
			if (pos.x > MAX_THUMBSTICK_VALUE)
				pos.x = MAX_THUMBSTICK_VALUE;
			else if (pos.x < -MAX_THUMBSTICK_VALUE)
				pos.x = -MAX_THUMBSTICK_VALUE;

			//adjust magnitude relative to the end of the dead zone
			if (pos.x < 0)
				pos.x += DEAD_ZONE;
			else
				pos.x -= DEAD_ZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			pos.x /= MAX_THUMBSTICK_VALUE - DEAD_ZONE;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			pos.x = 0.0f;
		}

		//check if the controller is outside a circular dead zone
		if (pos.y > DEAD_ZONE || pos.y < -DEAD_ZONE)
		{
			//clip the magnitude at its expected maximum value
			if (pos.y > MAX_THUMBSTICK_VALUE)
				pos.y = MAX_THUMBSTICK_VALUE;
			else if (pos.y < -MAX_THUMBSTICK_VALUE)
				pos.y = -MAX_THUMBSTICK_VALUE;

			//adjust magnitude relative to the end of the dead zone
			if (pos.y < 0)
				pos.y += DEAD_ZONE;
			else
				pos.y -= DEAD_ZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			pos.y /= MAX_THUMBSTICK_VALUE - DEAD_ZONE;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			pos.y = 0.0f;
		}

		return { pos.x, pos.y };
	}

	bool IsDown(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
	bool IsUp(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
	bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }
	bool HasLeftThumbStickMoved() const
	{
		const auto x = m_CurrentState.Gamepad.sThumbLX;
		const auto y = m_CurrentState.Gamepad.sThumbLY;

		if (x < -DEAD_ZONE || x > DEAD_ZONE
			|| y < -DEAD_ZONE || y > DEAD_ZONE)
		{
			return true;
		}
		return false;
	}
	bool HasRightThumbStickMoved() const
	{
		const auto x = m_CurrentState.Gamepad.sThumbRX;
		const auto y = m_CurrentState.Gamepad.sThumbRY;

		if (x < -DEAD_ZONE || x > DEAD_ZONE
			|| y < -DEAD_ZONE || y > DEAD_ZONE)
		{
			return true;
		}
		return false;
	}
	 
private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	int m_ControllerIndex{};
};

XInputController::XInputController(int controllerIndex)
{
	m_pImpl = std::make_unique<XInputControllerImpl>(controllerIndex);
}

void XInputController::Update()
{
	m_pImpl->Update();
}

int XInputController::GetIndex() const
{
	return m_pImpl->GetIndex();
}

glm::vec2 XInputController::GetNormalizedLeftThumbStickPos() const
{
	return m_pImpl->GetNormalizedLeftThumbStickPos();
}

bool XInputController::IsDown(ControllerButton button) const
{
	return m_pImpl->IsDown(static_cast<unsigned int>(button));
}

bool XInputController::IsUp(ControllerButton button) const
{
	return m_pImpl->IsUp(static_cast<unsigned int>(button));
}

bool XInputController::IsPressed(ControllerButton button) const
{
	return m_pImpl->IsPressed(static_cast<unsigned int>(button));
}

bool XInputController::HasLeftThumbStickMoved() const
{
	return m_pImpl->HasLeftThumbStickMoved();
}

bool XInputController::HasRightThumbStickMoved() const
{
	return m_pImpl->HasRightThumbStickMoved();
}
