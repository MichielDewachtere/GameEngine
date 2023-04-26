#ifndef COMMAND_H
#define COMMAND_H

namespace real
{
	class GameObject;
	class XInputController;

	class Command
	{
	public:
		explicit Command(GameObject* object) { m_pGameObject = object; }
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command& operator=(const Command& rhs) = delete;
		Command(Command&& other) = delete;
		Command& operator=(Command&& rhs) = delete;

		virtual void Execute() = 0;

		void SetInputController(XInputController* controller) { m_pController = controller; }
		void SetKeyBoardInput(int key) { m_Key = key; }

	protected:
		GameObject* GetOwner() const { return m_pGameObject; }
		XInputController* GetController() const { return m_pController; }
		int GetKeyBoardInput() const { return m_Key; }

	private:
		GameObject* m_pGameObject;
		XInputController* m_pController{ nullptr };
		int m_Key{ 0 };
	};

	class TestCommand final : public Command
	{
	public:
		explicit TestCommand(GameObject* object) : Command(object) {}

		void Execute() override;
	};
}

#endif // COMMAND_H