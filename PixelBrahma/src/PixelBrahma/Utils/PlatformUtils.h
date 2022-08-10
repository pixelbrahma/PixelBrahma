#pragma once

#include <string>

namespace PixelBrahma
{
	// File open and save menu items class
	class FileDialogues
	{
	public:

		// Open and save file functions

		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	// Time class
	class Time
	{
	public:

		// Get time function
		static float GetTime();
	};
}
