#include "epch.h"
#include "EditorNode.h"

std::unordered_map<std::type_index, EditorNode*> EditorNode::s_ClassRepresents;

static void DrawPin(const EditorNodePin& pin)
{
	ImNode::BeginPin((uintptr_t)pin.ID, pin.IsInput ? ImNode::PinKind::Input : ImNode::PinKind::Output);
	ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)GetPinColor(pin.Type));
	ImGui::Text(pin.Type == PinType::Execution ? ">>" : "->");
	ImGui::PopStyleColor();
	ImNode::EndPin();
}

static void DrawPinLabel(const EditorNodePin& pin)
{
	ImGui::PushStyleColor(ImGuiCol_Text, (ImU32)GetPinColor(pin.Type));
	ImGui::Text(pin.Label.c_str());
	ImGui::PopStyleColor();
}

EditorNode::EditorNode(const std::string& label, EditorNodeType type)
	: m_Label(label), m_ID(UUID()), m_Type(type)
{ }

void EditorNode::Render()
{
	ImGui::PushID(m_ID);

	ImNode::BeginNode((uintptr_t)m_ID);

	ImGui::BeginVertical("Node");
	ImGui::Spring();
	{
		ImGui::BeginHorizontal("Node header");

		ImGui::Spring(0);

		ImGui::BeginVertical("Input execution pins");
		for (const auto& pin : m_Pins) if (pin.Type == PinType::Execution && pin.IsInput) DrawPin(pin);
		ImGui::EndVertical();

		ImGui::BeginVertical("Input execution labels");
		for (const auto& pin : m_Pins) if (pin.Type == PinType::Execution && pin.IsInput) DrawPinLabel(pin);
		ImGui::EndVertical();

		ImGui::Spring(0.5f);

		ImGui::Text(m_Label.c_str());

		ImGui::Spring(1.0f);

		ImGui::BeginVertical("Output execution labels");
		for (const auto& pin : m_Pins) if (pin.Type == PinType::Execution && !pin.IsInput) DrawPinLabel(pin);
		ImGui::EndVertical();

		ImGui::BeginVertical("Output execution pins");
		for (const auto& pin : m_Pins) if (pin.Type == PinType::Execution && !pin.IsInput) DrawPin(pin);
		ImGui::EndVertical();

		ImGui::EndHorizontal();
	}

	{
		ImGui::BeginHorizontal("Node body");

		ImGui::Spring(0);

		ImGui::BeginVertical("Input pins");
		for (const auto& pin : m_Pins) if (pin.Type != PinType::Execution && pin.IsInput) DrawPin(pin);
		ImGui::EndVertical();

		ImGui::BeginVertical("Input labels");
		for (const auto& pin : m_Pins) if (pin.Type != PinType::Execution && pin.IsInput) DrawPinLabel(pin);
		ImGui::EndVertical();

		ImGui::BeginVertical("Render content");
		RenderContent();
		ImGui::EndVertical();

		ImGui::Spring(1);

		ImGui::BeginVertical("Output labels");
		for (const auto& pin : m_Pins) if (pin.Type != PinType::Execution && !pin.IsInput) DrawPinLabel(pin);
		ImGui::EndVertical();

		ImGui::BeginVertical("Output pins");
		for (const auto& pin : m_Pins) if (pin.Type != PinType::Execution && !pin.IsInput) DrawPin(pin);
		ImGui::EndVertical();

		ImGui::EndHorizontal();
	}

	ImGui::EndVertical();

	ImNode::EndNode();

	ImGui::PopID();
}

size_t EditorNode::AddPin(const EditorNodePin& pin)
{
	m_Pins.push_back(pin);
	return m_Pins.size() - 1;
}

void EditorNode::RemovePin(PinID pinID)
{
	for (size_t i = 0; i < m_Pins.size(); i++)
	{
		const auto& pin = m_Pins[i];
		if (pin.ID == pinID)
		{
			m_Pins.erase(m_Pins.begin() + 1);
			return;
		}
	}
}
