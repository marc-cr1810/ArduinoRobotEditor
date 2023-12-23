#pragma once

#include "Editors/Editor.h"
#include "Editors/NodeEditor/NodeGraph/NodeGraph.h"
#include "Editors/NodeEditor/Drawing/NewNodeContextMenu.h"

#include "ImGui/ImGuiCommon.h"

class NodeEditor : public Editor
{
public:
	NodeEditor();
	~NodeEditor();

	void OnUpdate() override;
	void OnRender() override;

	void UpdateNodes();
	void RenderContextMenus();

	void Edit(const char* filepath) override;
	void Save() override;
private:
	ImNode::EditorContext* m_EditorContext;
	Scope<NodeGraph> m_NodeGraph;

	Scope<NewNodeContextMenu> m_NewNodeMenu;
};