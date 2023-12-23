#include "epch.h"
#include "ContextMenu.h"

#include "ImGui/ImGuiCommon.h"

void ContextMenu::OnRender()
{
	ImNode::Suspend();

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