#include "epch.h"
#include "OutputWindow.h"

#include "Core/Application.h"
#include "ImGui/ImGuiCommon.h"

OutputWindow::OutputWindow()
{
	m_Outputs["General"];
	m_CurrentOutput = "General";
}

void OutputWindow::OnRender()
{
	if (m_Open)
	{
		if (ImGui::Begin("Output", &m_Open))
		{
			ImGui::Text("Show output from:");
			ImGui::SameLine();
			if (ImGui::BeginCombo("##outputOptions", m_CurrentOutput.c_str()))
			{
				for (auto& output : m_Outputs)
				{
					bool isSelected = m_CurrentOutput == output.first;
					if (ImGui::Selectable(output.first.c_str(), isSelected))
						m_CurrentOutput = output.first;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			// Reserve enough left-over height for 1 separator + 1 input text
			const float footerHightToReserve = ImGui::GetStyle().ItemSpacing.y;
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				for each (auto line in m_Outputs[m_CurrentOutput])
				{
					OutputLevel level = line.first;
					std::string str = line.second;
					ImVec4 color;
					bool has_color = false;
					if (level == OUTPUT_LEVEL_ERR) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }

					if (has_color)
						ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(str.c_str());
					if (has_color)
						ImGui::PopStyleColor();
				}

				// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
				// Using a scrollbar or mouse-wheel will take away from the bottom edge.
				if (m_ScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY(1.0f);
				m_ScrollToBottom = false;
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}

void OutputWindow::AddLog(const std::string& output, const std::string& line, bool change)
{
	AddLog(output, line, OUTPUT_LEVEL_INFO, change);
}

void OutputWindow::AddLog(const std::string& output, const std::string& line, OutputLevel level, bool change)
{
	m_Outputs[output].push_back(std::make_pair(level, line));
	if (change)
		m_CurrentOutput = output;
	m_ScrollToBottom = true;
}

void OutputWindow::Clear(const std::string& output)
{
	m_Outputs[output].clear();
}
