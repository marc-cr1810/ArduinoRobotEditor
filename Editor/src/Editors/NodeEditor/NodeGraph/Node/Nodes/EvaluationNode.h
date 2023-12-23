#pragma once

#include "Editors/NodeEditor/NodeGraph/Node/EditorNode.h"

class EvaluationNode : public EditorNode
{
public:
	using EditorNode::EditorNode;
};

class BoolEditorNode : public EvaluationNode
{
public:
	BoolEditorNode()
		: EvaluationNode("Bool", EditorNodeType::Bool)
	{
		AddPin(EditorNodePin::CreateOutputPin("", PinType::Bool));
	}

	bool GetValue() const { return m_Value; }
protected:
	virtual void RenderContent() override;
private:
	bool m_Value = false;
};

class FloatNEditorNode : public EvaluationNode
{
public:
	FloatNEditorNode(uint32_t numFloats, EditorNodeType nodeType, PinType pinType)
		: EvaluationNode("Float" + (numFloats == 1 ? "" : std::to_string(numFloats)), nodeType), m_NumValues(numFloats)
	{
		AddPin(EditorNodePin::CreateOutputPin("", pinType));

		for (uint32_t i = 0; i < 4; i++)
			m_Values[i] = 0.0f;
	}

	float GetFloat() const { ED_ASSERT(m_NumValues == 1); return m_Values[0]; }
	Float2 GetFloat2() const { ED_ASSERT(m_NumValues == 2); return Float2(m_Values[0], m_Values[1]); }
	Float3 GetFloat3() const { ED_ASSERT(m_NumValues == 3); return Float3{ m_Values[0], m_Values[1], m_Values[2] }; }
	Float4 GetFloat4() const { ED_ASSERT(m_NumValues == 4); return Float4{ m_Values[0], m_Values[1], m_Values[2], m_Values[3] }; }
protected:
	virtual void RenderContent() override;
private:
	uint32_t m_NumValues;
	float m_Values[4];
};

template<EditorNodeType nodeType, PinType pinType, uint32_t numFloats>
class FloatNEditorNodeT : public FloatNEditorNode
{
public:
	FloatNEditorNodeT()
		: FloatNEditorNode(numFloats, nodeType, pinType)
	{}

	static_assert(numFloats <= 4 && numFloats > 0);
};

class SplitVectorEditorNode : public EvaluationNode
{
public:
	SplitVectorEditorNode(EditorNodeType nodeType, PinType inputPinType, PinType outputPinType, unsigned numOutputs) :
		EvaluationNode("Split " + EditorNodePin::ToString(inputPinType), nodeType),
		m_NumOutputs(numOutputs)
	{
		static std::vector<std::string> outputLabels = { "x", "y", "z", "w" };
		m_InputVectorPin = AddPin(EditorNodePin::CreateInputPin("", inputPinType));
		for (unsigned i = 0; i < numOutputs; i++)
			m_OutputPins[i] = AddPin(EditorNodePin::CreateOutputPin(outputLabels[i], outputPinType));
	}

	unsigned GetOutputPinIndex(PinID pinID)
	{
		for (unsigned i = 0; i < m_NumOutputs; i++)
		{
			if (pinID == GetPins()[m_OutputPins[i]].ID)
				return i;
		}
		ED_ASSERT(0);
		return 0;
	}

	const EditorNodePin& GetInputVectorPin() const { return GetPins()[m_InputVectorPin]; }
private:
	unsigned m_NumOutputs;
	unsigned m_InputVectorPin;
	unsigned m_OutputPins[4];
};

template<EditorNodeType nodeType, PinType inputPinType, PinType outputPinType, unsigned numOutputs>
class SplitVectorEditorNodeT : public SplitVectorEditorNode
{
public:
	SplitVectorEditorNodeT() :
		SplitVectorEditorNode(nodeType, inputPinType, outputPinType, numOutputs)
	{ }

	static_assert(numOutputs <= 4 && numOutputs > 0);
};

enum class BinaryOperatorCategory
{
	Arithmetic,
	Logic,
	Comparison
};

class BinaryOperatorEditorNode : public EvaluationNode
{
public:
	BinaryOperatorEditorNode(EditorNodeType nodeType, BinaryOperatorCategory opCategory, std::string op, PinType pinType, PinType outputPinType)
		: EvaluationNode(EditorNodePin::ToString(pinType) + " operator", nodeType), m_OpCategory(opCategory), m_Op(op)
	{
		auto ops = GetOperatorOptions(m_OpCategory);
		ED_ASSERT(std::find(ops.begin(), ops.end(), op) != ops.end(), "Operator is not valid for the category!");

		m_PinA = AddPin(EditorNodePin::CreateInputPin("A", pinType));
		m_PinB = AddPin(EditorNodePin::CreateInputPin("B", pinType));
		AddPin(EditorNodePin::CreateOutputPin("", outputPinType));
	}

	const EditorNodePin& GetAPin() const { return GetPins()[m_PinA]; }
	const EditorNodePin& GetBPin() const { return GetPins()[m_PinB]; }
	const BinaryOperatorCategory& GetCategory() const { return m_OpCategory; }
	const std::string& GetOp() const { return m_Op; }
public:
	static std::string GetDefaultOperation(BinaryOperatorCategory opType)
	{
		switch (opType)
		{
		case BinaryOperatorCategory::Arithmetic: return "+";
		case BinaryOperatorCategory::Logic:		 return "AND";
		case BinaryOperatorCategory::Comparison: return "==";
		}
		return "";
	}

	static const std::vector<std::string> GetOperatorOptions(BinaryOperatorCategory opType)
	{
		static const std::vector<std::string> noOperators = {};
		static const std::vector<std::string> arithmeticOperators = { "+", "-", "*", "/" };
		static const std::vector<std::string> comparisonOperators = { "==", "!=", ">", "<", ">=", "<=" };
		static const std::vector<std::string> logicOperators = { "AND", "OR", "XOR" };

		const std::vector<std::string>* operators = nullptr;
		switch (opType)
		{
		case BinaryOperatorCategory::Arithmetic: return arithmeticOperators;
		case BinaryOperatorCategory::Logic: return logicOperators;
		case BinaryOperatorCategory::Comparison: return comparisonOperators;
		default:
			NOT_IMPLEMENTED;
		}
		return noOperators;
	}
protected:
	virtual void RenderContent() override;
private:
	PinID m_PinA;
	PinID m_PinB;
	BinaryOperatorCategory m_OpCategory;
	std::string m_Op;
};

template<EditorNodeType nodeType, BinaryOperatorCategory opCategory, PinType pinType, PinType outputPinType>
class BinaryOperatorEditorNodeT : public BinaryOperatorEditorNode
{
public:
	BinaryOperatorEditorNodeT() :
		BinaryOperatorEditorNode(nodeType, opCategory, GetDefaultOperation(opCategory), pinType, outputPinType)
	{}

	BinaryOperatorEditorNodeT(std::string op) :
		BinaryOperatorEditorNode(nodeType, opCategory, op, pinType, outputPinType)
	{}
};

class StringEditorNode : public EvaluationNode
{
public:
	StringEditorNode()
		: EvaluationNode("String", EditorNodeType::String)
	{
		AddPin(EditorNodePin::CreateOutputPin("", PinType::String));
	}

	std::string GetValue() const { return m_Value; }
protected:
	virtual void RenderContent() override;
private:
	std::string m_Value = "";
};

using BoolBinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::BoolBinaryOperator, BinaryOperatorCategory::Logic, PinType::Bool, PinType::Bool>;

using StringBinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::StringBinaryOperator, BinaryOperatorCategory::Arithmetic, PinType::String, PinType::String>;

using FloatEditorNode = FloatNEditorNodeT<EditorNodeType::Float, PinType::Float, 1>;
using FloatBinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::FloatBinaryOperator, BinaryOperatorCategory::Arithmetic, PinType::Float, PinType::Float>;
using FloatComparisonOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::FloatComparisonOperator, BinaryOperatorCategory::Comparison, PinType::Float, PinType::Bool>;

using Float2EditorNode = FloatNEditorNodeT<EditorNodeType::Float2, PinType::Float2, 2>;
using SplitFloat2EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat2, PinType::Float2, PinType::Float, 2>;
using Float2BinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::Float2BinaryOperator, BinaryOperatorCategory::Arithmetic, PinType::Float2, PinType::Float2>;

using Float3EditorNode = FloatNEditorNodeT<EditorNodeType::Float3, PinType::Float3, 3>;
using SplitFloat3EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat3, PinType::Float3, PinType::Float, 3>;
using Float3BinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::Float3BinaryOperator, BinaryOperatorCategory::Arithmetic, PinType::Float3, PinType::Float3>;

using Float4EditorNode = FloatNEditorNodeT<EditorNodeType::Float4, PinType::Float4, 4>;
using SplitFloat4EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat4, PinType::Float4, PinType::Float, 4>;
using Float4BinaryOperatorEditorNode = BinaryOperatorEditorNodeT<EditorNodeType::Float4BinaryOperator, BinaryOperatorCategory::Arithmetic, PinType::Float4, PinType::Float4>;