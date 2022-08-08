
-- PixelBrahma Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PixelBrahma/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/PixelBrahma/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/PixelBrahma/ThirdParty/ImGui"
IncludeDir["glm"] = "%{wks.location}/PixelBrahma/ThirdParty/glm"
IncludeDir["stb_image"] = "%{wks.location}/PixelBrahma/ThirdParty/stb_image"
IncludeDir["enTT"] = "%{wks.location}/PixelBrahma/ThirdParty/enTT/Include"
IncludeDir["yaml_cpp"] = "%{wks.location}/PixelBrahma/ThirdParty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/PixelBrahma/ThirdParty/ImGuizmo"
IncludeDir["shaderc"] = "%{wks.location}/PixelBrahma/ThirdParty/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/PixelBrahma/ThirdParty/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/PixelBrahma/ThirdParty/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"