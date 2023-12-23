#pragma once

#include "Core/UUID.h"
#include "Editors/NodeEditor/NodeGraph/Node/EditorNodePin.h"

#include <typeindex>

enum class EditorNodeType
{
	Invalid = -1,

	OnUpdate,
	OnStart,
	Bool,
	BoolBinaryOperator,
	String,
	StringBinaryOperator,
	Float,
	Float2,
	Float3,
	Float4,
	SplitFloat2,
	SplitFloat3,
	SplitFloat4,
	FloatComparisonOperator,
	FloatBinaryOperator,
	Float2BinaryOperator,
	Float3BinaryOperator,
	Float4BinaryOperator,
	Print,
	If
};

struct EditorNodeLink
{
	LinkID ID;
	PinID Start;
	PinID End;
};

class EditorNode
{
public:
	EditorNode(const std::string& label, EditorNodeType type);

	void Render();

	size_t AddPin(const EditorNodePin& pin);
	void RemovePin(PinID pinID);

	NodeID GetID() const { return m_ID; }
	EditorNodeType GetType() const { return m_Type; }
	const std::vector<EditorNodePin>& GetPins() const { return m_Pins; }
public:
	template<typename T>
	static EditorNode* GetClassRepresent()
	{
		if (s_ClassRepresents.find(typeid(T)) == s_ClassRepresents.end())
			s_ClassRepresents[typeid(T)] = new T{};
		return s_ClassRepresents[typeid(T)];
	}
protected:
	virtual void RenderContent() {}
public:
	static std::unordered_map<std::type_index, EditorNode*> s_ClassRepresents;
private:
	std::string m_Label;
	EditorNodeType m_Type;
	NodeID m_ID = 0;

	std::vector<EditorNodePin> m_Pins;
};