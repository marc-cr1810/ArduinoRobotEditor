
-- Editor Dependencies

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Editor/vendor/stb_image"
IncludeDir["GLFW"] = "%{wks.location}/Editor/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Editor/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Editor/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Editor/vendor/glm"
IncludeDir["imgui_nodes"] = "%{wks.location}/Editor/vendor/imgui_nodes"

LibraryDir = {}

Library = {}

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"