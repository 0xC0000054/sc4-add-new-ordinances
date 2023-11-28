////////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-add-new-ordinances, a DLL Plugin for SimCity 4
// that experiments with adding new ordinances to the game.
//
// Copyright (c) 2023 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////////

#include "Logger.h"
#include <Windows.h>

namespace
{
	std::string GetTimeStamp()
	{
		char buffer[1024]{};

		SYSTEMTIME time;

		GetLocalTime(&time);

		std::snprintf(
			buffer,
			sizeof(buffer),
			"[%hu:%hu:%hu.%hu] ",
			time.wHour,
			time.wMinute,
			time.wSecond,
			time.wMilliseconds);

		return std::string(buffer);
	}

#ifdef _DEBUG
	void PrintLineToDebugOutput(const char* line)
	{
		OutputDebugStringA(line);
		OutputDebugStringA("\n");
	}
#endif // _DEBUG
}

Logger& Logger::GetInstance()
{
	static Logger logger;

    return logger;
}

Logger::Logger() : initialized(false), logFile()
{
}

Logger::~Logger()
{
	initialized = false;
}

void Logger::Init(std::filesystem::path logFilePath)
{
	if (!initialized)
	{
		initialized = true;

		logFile.open(logFilePath, std::ofstream::out | std::ofstream::trunc);
	}
}

void Logger::WriteLine(const char* const message)
{
#ifdef _DEBUG
	PrintLineToDebugOutput(message);
#endif // _DEBUG

	if (initialized && logFile)
	{
		logFile << GetTimeStamp() << message << std::endl;
	}
}

void Logger::WriteLineFormatted(const char* const format, ...)
{
	va_list args;
	va_start(args, format);

	va_list argsCopy;
	va_copy(argsCopy, args);

	int formattedStringLength = std::vsnprintf(nullptr, 0, format, argsCopy);

	va_end(argsCopy);

	if (formattedStringLength > 0)
	{
		size_t formattedStringLengthWithNull = static_cast<size_t>(formattedStringLength) + 1;

		std::unique_ptr<char[]> buffer = std::make_unique_for_overwrite<char[]>(formattedStringLengthWithNull);

		std::vsnprintf(buffer.get(), formattedStringLengthWithNull, format, args);

		WriteLine(buffer.get());
	}

	va_end(args);
}
