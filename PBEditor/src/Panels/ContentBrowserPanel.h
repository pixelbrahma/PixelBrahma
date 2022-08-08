#pragma once

#include <filesystem>

#include "PixelBrahma/Renderer/Shading/Texture.h"

namespace PixelBrahma
{
	// Content browser panel class
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		// ImGui render function
		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}
