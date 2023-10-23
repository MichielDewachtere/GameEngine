#ifndef MINIGIN_H
#define MINIGIN_H

#include <string>
#include <functional>
#include <SDL_pixels.h>

namespace real
{
	struct WindowSettings
	{
		std::string title{};
		int width{}, height{};
		SDL_Color background{};
	};

	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath, const WindowSettings& window);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}

#endif // MINIGIN_H