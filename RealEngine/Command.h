#ifndef COMMAND_H
#define COMMAND_H

namespace real
{
	class GameObject;

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

	protected:
		GameObject* GetOwner() const { return m_pGameObject; }

	private:
		GameObject* m_pGameObject;
	};

	class TestCommand final : public Command
	{
	public:
		explicit TestCommand(GameObject* object) : Command(object) {}

		void Execute() override;
	};
}

#endif // COMMAND_H