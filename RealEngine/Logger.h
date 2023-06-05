#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <string>
#include <iostream>

namespace real
{
	class Logger
	{
	public:
		//TODO : LOG TO FILE?
		/**
		 * \brief A simple logger to log info.
		 * \tparam Args 
		 * \param message Message that should be displayed. Use {} to attach your arguments. 
		 * \param args The variables you want to display in your message.
		 */
		template <typename... Args>
		static void LogInfo(const std::string& message, Args&&... args)
		{
			std::ostringstream oss;
			oss << GetTime() << " [INFO] ";
			LogImpl(oss, message, std::forward<Args>(args)...);
			std::cout << "\033[36m" << oss.str() << "\033[0m\n";
		}
		/**
		 * \brief A simple logger to log a warning.
		 * \tparam Args
		 * \param message Message that should be displayed. Use {} to attach your arguments.
		 * \param args The variables you want to display in your message.
		 */
		template <typename... Args>
		static void LogWarning(const std::string& message, Args&&... args)
		{
			std::ostringstream oss;
			oss << GetTime() << " [INFO] ";
			LogImpl(oss, message, std::forward<Args>(args)...);
			std::cout << "\033[33m" << oss.str() << "\033[0m\n";
		}
		/**
		* \brief A simple logger to log an error.
		* \tparam Args
		* \param message Message that should be displayed. Use {} to attach your arguments.
		* \param args The variables you want to display in your message.
		*/
		template <typename... Args>
		static void LogError(const std::string& message, Args&&... args)
		{
			std::ostringstream oss;
			oss << GetTime() << " [INFO] ";
			LogImpl(oss, message, std::forward<Args>(args)...);
			std::cout << "\033[31m" << oss.str() << "\033[0m\n";
		}

	private:
		template<typename T, typename... Args>
		static void LogImpl(std::ostringstream& oss, const std::string& message, T arg, Args&&... args)
		{
			const size_t pos = message.find("{}");
			if (pos == std::string::npos)
			{
				oss << message;
				LogImpl(oss, message, std::forward<Args>(args)...);
			}
			else
			{
				oss << message.substr(0, pos) << arg;
				LogImpl(oss, message.substr(pos + 2), std::forward<Args>(args)...);
			}
		}

		static void LogImpl(std::ostringstream& oss, const std::string& message)
		{
			oss << message;
		}

		static std::string GetTime()
		{
			const auto now = std::chrono::system_clock::now();
			const std::time_t time = std::chrono::system_clock::to_time_t(now);

			char timeStr[26];
			ctime_s(timeStr, sizeof timeStr, &time);
			timeStr[strlen(timeStr) - 1] = '\0'; // remove newline character

			return "[" + std::string(timeStr) + "]";
		}
	};
}
#endif // LOGGER_H