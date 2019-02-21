#include "ExpressionNode.hpp"

#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "ExpressionCalculator.hpp"
#include "ExpressionEditor.hpp"

NE::DynamicSerializationInfo	ExpressionNode::serializationInfo (NE::ObjectId ("{63CF9382-20BE-48EA-B185-DE8A6A23DBF6}"), NE::ObjectVersion (1), ExpressionNode::CreateSerializableInstance);

ExpressionNode::ExpressionNode () :
	ExpressionNode (L"", NUIE::Point ())
{

}

ExpressionNode::ExpressionNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position),
	expression (L"X + Y + Z")
{

}

void ExpressionNode::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("z"), L"Z", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), L"Result")));
}

NE::ValueConstPtr ExpressionNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr x = EvaluateInputSlot (NE::SlotId ("x"), env);
	NE::ValueConstPtr y = EvaluateInputSlot (NE::SlotId ("y"), env);
	NE::ValueConstPtr z = EvaluateInputSlot (NE::SlotId ("z"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y) || !NE::IsComplexType<NE::NumberValue> (z)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	bool success = valueCombination->CombineValues ({x, y, z}, [&] (const NE::ValueCombination& combination) {
		BoostOperations::IdentifierMap identifierMap;
		identifierMap.insert ({ L"X", NE::NumberValue::ToDouble (combination.GetValue (0))});
		identifierMap.insert ({ L"Y", NE::NumberValue::ToDouble (combination.GetValue (1))});
		identifierMap.insert ({ L"Z", NE::NumberValue::ToDouble (combination.GetValue (2))});
		double expResult = 0.0;
		if (!BoostOperations::EvaluateExpression (expression, identifierMap, expResult)) {
			return false;
		}
		result->Push (NE::ValuePtr (new NE::DoubleValue (expResult)));
		return true;
	});

	if (!success) {
		return nullptr;
	}

	return result;
}

void ExpressionNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	class SetExpressionCommand : public NUIE::NodeCommand
	{
	public:
		SetExpressionCommand () :
			NUIE::NodeCommand (L"Set Expression", false)
		{

		}

		virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) override
		{
			return NE::Node::IsTypeConst<ExpressionNode> (uiNode);
		}

		virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& /*uiEnvironment*/, NUIE::UINodePtr& uiNode) override
		{
			std::shared_ptr<ExpressionNode> expNode = std::dynamic_pointer_cast<ExpressionNode> (uiNode);
			std::wstring expression = expNode->GetExpression ();

			ExpressionEditorDialog expEditor (nullptr, expression);
			if (expEditor.ShowModal () == wxID_OK) {
				expNode->SetExpression (expEditor.GetExpression ());
				uiManager.InvalidateNodeValue (expNode);
				uiManager.InvalidateNodeDrawing (expNode);
			}
		}

	private:
		bool enable;
	};

	BI::BasicUINode::RegisterCommands (commandRegistrator);
	commandRegistrator.RegisterNodeCommand (NUIE::NodeCommandPtr (new SetExpressionCommand ()));
}


void ExpressionNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<ExpressionNode, NE::DoubleValue> (parameterList, L"X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ExpressionNode, NE::DoubleValue> (parameterList, L"Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ExpressionNode, NE::DoubleValue> (parameterList, L"Z", NUIE::ParameterType::Double, NE::SlotId ("z"));
}

NE::Stream::Status ExpressionNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	inputStream.Read (expression);
	return inputStream.GetStatus ();
}

NE::Stream::Status ExpressionNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	outputStream.Write (expression);
	return outputStream.GetStatus ();
}

const std::wstring & ExpressionNode::GetExpression () const
{
	return expression;
}

void ExpressionNode::SetExpression (const std::wstring& newExpression)
{
	expression = newExpression;
}
