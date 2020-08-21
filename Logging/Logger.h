#pragma once
#include <ctime>
#include <iostream>
#include <iomanip>
#include<string>
#ifdef LOGGING_EXPORTS
#define API_LOGGER __declspec(dllexport)
#else
#define API_LOGGER __declspec(dllimport)
#endif//LOGGING_EXPORTS
#pragma warning(disable::4996)

class API_LOGGER Logger
{
public:
	enum class Level
	{
		Debug,
		Info,
		Warning,
		Error
	};
private:
	std::ostream& m_out;

public:
	Level m_minLogLevel;
	Logger(std::ostream& out, Level minLogLevel = Level::Warning);
	template<class ... Args>
	void Logi(Level level, Args&& ... params)
	{
		if (level < m_minLogLevel)
			return;
		switch (level)
		{
		case Logger::Level::Debug:
		{
			std::cout << "[Debug]";
			std::time_t crtTime = std::time(nullptr);
			std::cout << '[' << std::put_time(std::localtime(&crtTime), "%Y-%m-%d %H:%M:%S") << ']';
			((std::cout << ' ' << std::forward<Args>(params)), ...);
			std::cout << '\n';
			break;
		}
		case Logger::Level::Info:
		{
			std::cout << "[Info]";
			std::time_t crtTime = std::time(nullptr);
			std::cout << '[' << std::put_time(std::localtime(&crtTime), "%Y-%m-%d %H:%M:%S") << ']';
			((std::cout << ' ' << std::forward<Args>(params)), ...);
			std::cout << '\n';
			break;
		}
		case Logger::Level::Warning:
		{
			std::cout << "[Warning]";
			std::time_t crtTime = std::time(nullptr);
			std::cout << '[' << std::put_time(std::localtime(&crtTime), "%Y-%m-%d %H:%M:%S") << ']';
			((std::cout << ' ' << std::forward<Args>(params)), ...);
			std::cout << '\n';
			break;
		}
		case Logger::Level::Error:
		{
			std::cout << "[Error]";
			std::time_t crtTime = std::time(nullptr);
			std::cout << '[' << std::put_time(std::localtime(&crtTime), "%Y-%m-%d %H:%M:%S") << ']';
			((std::cout << ' ' << std::forward<Args>(params)), ...);
			std::cout << '\n';
			break;
		}
		}
	}
	~Logger();
};

