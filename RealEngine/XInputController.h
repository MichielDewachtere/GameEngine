#ifndef XINPUTCONTROLLER_H
#define XINPUTCONTROLLER_H

#include <glm/vec2.hpp>

namespace real
{
	class XInputController final
	{
	private:
		class XInputControllerImpl;
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
			ButtonDown = 0x1000,
			ButtonRight = 0x2000,
			ButtonLeft = 0x4000,
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

		explicit XInputController(uint8_t controllerIndex);
		~XInputController();
		XInputController(const XInputController& other) = delete;
		XInputController operator=(const XInputController& rhs) = delete;
		XInputController(XInputController&& other) = delete;
		XInputController operator=(XInputController&& rhs) = delete;

		void Update() const;

		uint8_t GetIndex() const;

		glm::vec2 GetNormalizedLeftThumbStickPos() const;

		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;
		bool HasLeftThumbStickMoved() const;
		bool HasRightThumbStickMoved() const;

	};
}

#endif // XINPUTCONTROLLER_H