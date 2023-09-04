#include "epch.h"
#include "ProjectExplorer.h"

#include "Core/Application.h"

ProjectExplorer::ProjectExplorer(Ref<Project> project)
	: m_Project(project)
{
}

static void EnterDirectory(std::filesystem::path rootPath, std::filesystem::path directory)
{
	std::vector<std::filesystem::directory_entry> files;

	for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
	{
		if (!directoryEntry.is_directory())
		{
			files.push_back(directoryEntry);
			continue;
		}

		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, rootPath);
		std::string filenameString = path.filename().string();

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (ImGui::TreeNodeEx(filenameString.c_str(), nodeFlags) && directoryEntry.is_directory())
		{
			EnterDirectory(rootPath, path);
			ImGui::TreePop();
		}
	}

	for each (auto& file in files)
	{
		const auto& path = file.path();
		auto relativePath = std::filesystem::relative(path, rootPath);
		std::string filenameString = path.filename().string();
		std::string extension = relativePath.extension().string();

		if (extension == ".areproj")
			continue;

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::TreeNodeEx(filenameString.c_str(), nodeFlags);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			Application::Get().OpenEditor(path.string().c_str());
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
