#include "epch.h"
#include "EvaluationNode.h"

#include <misc/cpp/imgui_stdlib.h>
#include <misc/cpp/imgui_stdlib.cpp>

void BoolEditorNode::RenderContent()
{
    ImGui::Checkbox("", &m_Value);
}

void FloatNEditorNode::RenderContent()
{
    const std::string valueNames[] = { "X", "Y", "Z", "W" };

    for (unsigned i = 0; i < m_NumValues; i++)
    {
        ImGui::SetNextItemWidth(50.0f);
        ImGui::DragFloat(valueNames[i].c_str(), &m_Values[i], 0.05f, 0.0f, 0.0f, "%.5f");
    }
}

void BinaryOperatorEditorNode::RenderContent()
{
    ImGui::Text(m_Op.c_str());
}

void StringEditorNode::RenderContent()
{
    ImGui::SetNextItemWidth(150.0f);
    ImGui::InputText("", &m_Value);
}
