#pragma once

#include "Editors/NodeEditor/NodeGraph/Node/EditorNode.h"
#include "Editors/NodeEditor/NodeGraph/Node/EditorNodePin.h"

#include "Editors/NodeEditor/NodeGraph/Node/Nodes/ExecutionNode.h"

class NodeGraph
{
public:
	void AddNode(EditorNode* node);
	void AddLink(const EditorNodeLink& link);

	void RemoveNode(NodeID nodeID);
	void RemoveLink(LinkID linkID);
	void RemovePin(PinID pinID);

	EditorNodePin GetPinByID(PinID pinID) const;
	EditorNode* GetPinOwner(PinID pinID) const;

	PinID GetInputPinFromOutput(PinID outputPinID) const;
	PinID GetOutputPinForInput(PinID inputPinID) const;

	OnStartEditorNode* GetOnStartNode() const;
	OnUpdateEditorNode* GetOnUpdateNode() const;

	size_t GetNodeCount() const { return m_Nodes.size(); }
	size_t GetLinkCount() const { return m_Links.size(); }

	template<typename Fn>
	void ForEachNode(Fn& fn)
	{
		for (const auto& n : m_Nodes)
		{
			fn(n.second.get());
		}
	}

	template<typename Fn>
	void ForEachLink(Fn& fn)
	{
		for (const auto& l : m_Links)
		{
			fn(l.second);
		}
	}
public:
	static NodeGraph* CreateDefault();
private:
	std::unordered_map<NodeID, Scope<EditorNode>> m_Nodes;
	std::unordered_map<LinkID, EditorNodeLink> m_Links;
};