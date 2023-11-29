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

#pragma once

#include <filesystem>
#include <fstream>

enum class LogOptions : int32_t
{
	None = 0,
	Errors = 1 << 0,
	OrdinanceAPI = 1 << 1,
	OrdinancePropertyAPI = 1 << 2,
	All = Errors | OrdinanceAPI | OrdinancePropertyAPI
};

inline LogOptions operator|(LogOptions lhs, LogOptions rhs)
{
	return static_cast<LogOptions>(
		static_cast<std::underlying_type<LogOptions>::type>(lhs) |
		static_cast<std::underlying_type<LogOptions>::type>(rhs)
		);
}

inline LogOptions operator&(LogOptions lhs, LogOptions rhs)
{
	return static_cast<LogOptions>(
		static_cast<std::underlying_type<LogOptions>::type>(lhs) &
		static_cast<std::underlying_type<LogOptions>::type>(rhs)
		);
}

class Logger
{
public:

	static Logger& GetInstance();

	void Init(std::filesystem::path logFilePath, LogOptions logLevel);

	void WriteLogFileHeader(const char* const message);

	void WriteLine(LogOptions level, const char* const message);

	void WriteLineFormatted(LogOptions level, const char* const format, ...);

private:

	Logger();
	~Logger();

	void WriteLineCore(const char* const message);

	bool initialized;
	LogOptions logOptions;
	std::ofstream logFile;
};

