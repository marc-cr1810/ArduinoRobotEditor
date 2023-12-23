#pragma once

#include "Editors/NodeEditor/NodeGraph/Node/EditorNode.h"
#include "Editors/NodeEditor/NodeGraph/Node/EditorNodePin.h"


class ExecutionEditorNode : public EditorNode
{
public:
	ExecutionEditorNode(const std::string& label, EditorNodeType type, bool input = true)
		: EditorNode(label, type)
	{
		if (input)
			m_PinInput = AddPin(EditorNodePin::CreateInputPin("", PinType::Execution));
		m_PinOutput = AddPin(EditorNodePin::CreateOutputPin("", PinType::Execution));
	}

	const EditorNodePin& GetInput() const { return GetPins()[m_PinInput]; }
	const EditorNodePin& GetOutput() const { return GetPins()[m_PinOutput]; }
private:
	size_t m_PinInput;
	size_t m_PinOutput;
};

class OnStartEditorNode : public ExecutionEditorNode
{
public:
	OnStartEditorNode() : ExecutionEditorNode("On Start", EditorNodeType::OnStart, false) {}
};

class OnUpdateEditorNode : public ExecutionEditorNode
{
public:
	OnUpdateEditorNode()
		: ExecutionEditorNode("On Update", EditorNodeType::OnUpdate, false)
	{
		AddPin(EditorNodePin::CreateOutputPin("DT", PinType::Float));
	}
};

class PrintEditorNode : public ExecutionEditorNode
{
public:
	PrintEditorNode()
		: ExecutionEditorNode("Print", EditorNodeType::Print)
	{
		m_PinInput = AddPin(EditorNodePin::CreateInputPin("", PinType::Generic));
	}

	const EditorNodePin& GetInput() const { return GetPins()[m_PinInput]; }
private:
	size_t m_PinInput;
};

class IfEditorNode : public ExecutionEditorNode
{
public:
	IfEditorNode() :
		ExecutionEditorNode("If", EditorNodeType::If)
	{
		m_ExectuionPinElse = AddPin(EditorNodePin::CreateOutputPin("ELSE", PinType::Execution));
		m_ConditionPin = AddPin(EditorNodePin::CreateInputPin("Condition", PinType::Bool));
	}

	const EditorNodePin& GetExecutionElse() const { return GetPins()[m_ExectuionPinElse]; }
	const EditorNodePin& GetConditionPin() const { return GetPins()[m_ConditionPin]; }
private:
	unsigned m_ExectuionPinElse;
	unsigned m_ConditionPin;
};