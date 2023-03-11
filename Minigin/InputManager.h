#pragma once
#include "Singleton.h"

namespace real
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
