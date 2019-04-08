#include "PrismNode.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "Basic3DNodeValues.hpp"
#include "MaterialNode.hpp"
#include "PolygonEditor.hpp"
#include "Triangulation.hpp"
#include "BasicShapes.hpp"

NE::DynamicSerializationInfo	PrismNode::serializationInfo (NE::ObjectId ("{9D1E49DE-FD64-4079-A75B-DE1900FD2C2F}"), NE::ObjectVersion (1), PrismNode::CreateSerializableInstance);

PrismNode::PrismNode () :
	PrismNode (L"", NUIE::Point ())
{

}

PrismNode::PrismNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void PrismNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));

	NE::ListValuePtr basePointsDefaultValue (new NE::ListValue ());
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (0.0, 0.0))));
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (2.0, 0.0))));
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (2.0, 2.0))));
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (1.0, 2.0))));
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (1.0, 1.0))));
	basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (glm::vec2 (0.0, 1.0))));

	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("basepoints"), L"Base Points", basePointsDefaultValue, NE::OutputSlotConnectionMode::Multiple)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("height"), L"Height", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr PrismNode::Calculate (NE::EvaluationEnv& env) const
{
	class CGALTriangulator : public Modeler::Triangulator
	{
	public:
		virtual bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result) override
		{
			return CGALOperations::TriangulatePolygon (points, result);
		}
	};

	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr basePointsValue = NE::FlattenValue (EvaluateInputSlot (NE::SlotId ("basepoints"), env));
	NE::ValueConstPtr heightValue = EvaluateInputSlot (NE::SlotId ("height"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<Point2DValue> (basePointsValue) || !NE::IsComplexType<NE::NumberValue> (heightValue)) {
		return nullptr;
	}

	Modeler::TriangulatorPtr triangulator (new CGALTriangulator ());
	std::vector<glm::dvec2> basePoints;
	NE::FlatEnumerate (basePointsValue, [&] (const NE::ValueConstPtr& value) {
		basePoints.push_back (Point2DValue::Get (value));
	});

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, heightValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (new Modeler::PrismShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			basePoints,
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			triangulator
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void PrismNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	class SetBasePolygonCommand : public NUIE::NodeCommand
	{
	public:
		SetBasePolygonCommand () :
			NUIE::NodeCommand (L"Set Base Polygon", false)
		{

		}

		virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) override
		{
			return NE::Node::IsTypeConst<PrismNode> (uiNode);
		}

		virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& /*uiEnvironment*/, NUIE::UINodePtr& uiNode) override
		{
			std::shared_ptr<PrismNode> prismNode = std::dynamic_pointer_cast<PrismNode> (uiNode);
			NE::InputSlotPtr inputSlot = prismNode->GetInputSlot (NE::SlotId ("basepoints"));
			
			std::vector<glm::dvec2> initPolygon;
			NE::ValueConstPtr defaultPolygon = NE::FlattenValue (inputSlot->GetDefaultValue ());
			if (NE::IsComplexType<Point2DValue> (defaultPolygon)) {
				NE::FlatEnumerate (defaultPolygon, [&] (const NE::ValueConstPtr& value) {
					initPolygon.push_back (Point2DValue::Get (value));
				});
			}

			PolygonEditorDialog polygonEditor (nullptr, initPolygon);
			if (polygonEditor.ShowModal () == wxID_OK && polygonEditor.HasPolygon ()) {
				NE::ListValuePtr basePointsDefaultValue (new NE::ListValue ());
				for (const glm::dvec2& point : polygonEditor.GetPolygon ()) { 
					basePointsDefaultValue->Push (NE::ValuePtr (new Point2DValue (point)));
				}
				inputSlot->SetDefaultValue (basePointsDefaultValue);

				uiManager.InvalidateNodeValue (prismNode);
				uiManager.InvalidateNodeDrawing (prismNode);
			}
		}

	private:
		bool enable;
	};

	ShapeNode::RegisterCommands (commandRegistrator);
	commandRegistrator.RegisterNodeCommand (NUIE::NodeCommandPtr (new SetBasePolygonCommand ()));
}

void PrismNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<PrismNode, NE::FloatValue> (parameterList, L"Height", NUIE::ParameterType::Float, NE::SlotId ("height"));
}

NE::Stream::Status PrismNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PrismNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}
