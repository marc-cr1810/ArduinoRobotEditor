#include "epch.h"
#include "ContextMenu.h"

#include "ImGui/ImGuiCommon.h"

void ContextMenu::OnRender()
{
	ImNode::Suspend();
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	//if (ImGui::BeginPopup("Node Context Menu"))
	//{
	//	ImGui::TextUnformatted("Node Context Menu");
	//	ImGui::EndPopup();
	//}
	//ImGui::PopStyleVar();

	/*if (m_Open)
	{
		ImGui::OpenPopup(m_ID.c_str());
		m_Open = false;
	}

	if (ImGui::BeginPopup(m_ID.c_str()))
	{
		DrawContent();
		ImGui::EndPopup();
	}*/

	ImNode::Resume();
}