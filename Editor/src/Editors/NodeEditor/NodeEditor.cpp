#include "epch.h"
#include "NodeEditor.h"

NodeEditor::NodeEditor()
{
	ImNode::Config config;
	m_EditorContext = ImNode::CreateEditor(&config);

    m_NodeGraph = CreateScope<NodeGraph>();

    m_NewNodeMenu = CreateScope<NewNodeContextMenu>("New Node Menu", *m_NodeGraph.get());
}

NodeEditor::~NodeEditor()
{
	ImNode::DestroyEditor(m_EditorContext);
}

void NodeEditor::OnUpdate()
{
}

void NodeEditor::OnRender()
{
	ImNode::SetCurrentEditor(m_EditorContext);
	ImNode::Begin("Node Editor", ImVec2(0.0, 0.0f));

    UpdateNodes();

    m_NodeGraph->ForEachNode([](EditorNode* node) { node->Render(); });

    m_NodeGraph->ForEachLink([this](const EditorNodeLink& link) {
        const auto& outputPin = m_NodeGraph->GetPinByID(link.Start);
        const bool isExecution = outputPin.Type == PinType::Execution;
        ImNode::Link((uintptr_t)link.ID, (uintptr_t)link.Start, (uintptr_t)link.End, GetPinColor(outputPin.Type), isExecution ? 3.0f : 1.0f);
        });

    RenderContextMenus();

	ImNode::End();
	ImNode::SetCurrentEditor(nullptr);
}

void NodeEditor::UpdateNodes()
{
    ImNode::PinId startPinID, endPinID, pin;
    ImNode::LinkId link;
    ImNode::NodeId node;

    if (ImNode::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        if (ImNode::QueryNewLink(&startPinID, &endPinID))
        {
            if (startPinID && endPinID)
            {
                auto startPin = m_NodeGraph->GetPinByID(startPinID.Get());
                auto endPin = m_NodeGraph->GetPinByID(endPinID.Get());

                // Make sure start is always output pin
                if (startPin.IsInput)
                {
                    const auto tmpPin = startPin;
                    startPin = endPin;
                    endPin = tmpPin;
                }

                bool validLink = startPin.ID != endPin.ID;
                validLink = validLink && startPin.IsInput != endPin.IsInput;
                validLink = validLink && (startPin.Type == endPin.Type || IsGenericPinType(startPin.Type, endPin.Type));

                if (!validLink)
                {
                    ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                }
                else if (ImNode::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                {
                    m_NodeGraph->AddLink(EditorNodeLink{ UUID(), startPin.ID, endPin.ID });
                }
            }
        }

        if (ImNode::QueryNewNode(&pin))
        {
            if (ImNode::AcceptNewItem())
            {
                m_NewNodeMenu->Open(pin.Get());
            }
        }
    }
    ImNode::EndCreate();

    if (ImNode::BeginDelete())
    {
        while (ImNode::QueryDeletedLink(&link))
        {
            if (ImNode::AcceptDeletedItem())
            {
                m_NodeGraph->RemoveLink(link.Get());
            }
        }

        while (ImNode::QueryDeletedNode(&node))
        {
            if (node.Get() == m_NodeGraph->GetOnStartNode()->GetID() ||
                node.Get() == m_NodeGraph->GetOnUpdateNode()->GetID())
            {
                ImNode::RejectDeletedItem();
            }
            else if (ImNode::AcceptDeletedItem())
            {
                m_NodeGraph->RemoveNode(node.Get());
            }
        }
    }
    ImNode::EndDelete();

    // Open context menus
    if (ImNode::ShowBackgroundContextMenu())
        m_NewNodeMenu->Open(0);
}

void NodeEditor::RenderContextMenus()
{
    m_NewNodeMenu->OnRender();
}

void NodeEditor::Edit(const char* filepath)
{
	m_Filepath = filepath;
}

void NodeEditor::Save()
{
}
