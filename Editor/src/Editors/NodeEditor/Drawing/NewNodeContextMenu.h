#pragma once

#include "Editors/NodeEditor/Drawing/ContextMenu.h"
#include "Editors/NodeEditor/NodeGraph/NodeGraph.h"

#include "Editors/NodeEditor/NodeGraph/Node/EditorNode.h"
#include "Editors/NodeEditor/NodeGraph/Node/EditorNodePin.h"
#include "Editors/NodeEditor/NodeGraph/Node/Nodes/EvaluationNode.h"
#include "Editors/NodeEditor/NodeGraph/Node/Nodes/ExecutionNode.h"

class NewNodeContextMenu : public ContextMenu
{
public:
	NewNodeContextMenu(const std::string& id, NodeGraph& nodeGraph)
		: ContextMenu(id), m_NodeGraph(nodeGraph)
	{}

	void Open() = delete;
	void Open(PinID draggedPin)
	{
		ContextMenu::Open();
		m_DraggedPin = draggedPin;
	}
protected:
	void DrawContent() override;
private:
	NodeGraph& m_NodeGraph;
	PinID m_DraggedPin = 0;
};