#include "epch.h"
#include "ProjectExplorer.h"

#include "Core/Application.h"

ProjectExplorer::ProjectExplorer(Ref<Project> project)
	: m_Project(project)
{
}

static void EnterDirectory(std::filesystem::path rootPath, std::filesystem::path directory)
{
	for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
	{
		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, rootPath);
		std::string filenameString = path.filename().string();
		std::string extension = relativePath.extension().string();

		if (extension == ".areproj")
			continue;

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (!directoryEntry.is_directory() && relativePath.has_extension())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		if (ImGui::TreeNodeEx(filenameString.c_str(), nodeFlags) && directoryEntry.is_directory())
		{
			EnterDirectory(rootPath, path);
			ImGui::TreePop();
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && !directoryEntry.is_directory())
		{
			Application::Get().OpenEditor(path.string().c_str());
			LOG_INFO(path.string());
		}
	}
}

void ProjectExplorer::OnRender()
{
	if (m_Open)
	{
		if (ImGui::Begin("Project Explorer", &m_Open))
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (m_Project->m_IncludeFiles.size() == 0)
				flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			else
				flags |= ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::TreeNodeEx(m_Project->m_Name.c_str(), flags))
			{
				auto projDir = std::filesystem::path(m_Project->m_Directory);
				EnterDirectory(projDir, projDir);
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}
