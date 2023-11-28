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

class Logger
{
public:

	static Logger& GetInstance();

	void Init(std::filesystem::path logFilePath);

	void WriteLine(const char* const message);

	void WriteLineFormatted(const char* const format, ...);

private:

	Logger();
	~Logger();

	bool initialized;
	std::ofstream logFile;
};

