#pragma once

#include <string>
#include <optional>

namespace PixelBrahma
{
	// File open and save menu items class
	class FileDialogues
	{
	public:

		// Open and save file functions
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};
}
