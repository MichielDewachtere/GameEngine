#ifndef XINPUTCONTROLLER_H
#define XINPUTCONTROLLER_H

#include "stdafx.h"

namespace real
{
	class XInputController final
	{
	private:
		class XInputControllerImpl;
		//std::unique_ptr<XInputControllerImpl> m_pImpl;
		XInputControllerImpl* m_pImpl;

	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			//LeftTrigger = 0x0400,
			//RightTrigger = 0x0800,
			ButtonDown = 0x1000,
			ButtonLeft = 0x2000,
			ButtonRight = 0x4000,
			ButtonUp = 0x8000
		};

		enum class InputType
		{
			pressed,
			down,
			up,
			leftThumbMoved,
			rightThumbMoved
		};

		explicit XInputController(int controllerIndex);
		~XInputController() /*= default*/;
		XInputController(const XInputController& other) = delete;
		XInputController operator=(const XInputController& rhs) = delete;
		XInputController(XInputController&& other) = delete;
		XInputController operator=(XInputController&& rhs) = delete;

		void Update();

		int GetIndex() const;

		glm::vec2 GetNormalizedLeftThumbStickPos() const;

		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;
		bool HasLeftThumbStickMoved() const;
		bool HasRightThumbStickMoved() const;

	};
}

#endif // XINPUTCONTROLLER_H