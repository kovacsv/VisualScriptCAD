#include "NodeRegistry.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "BI_BuiltInNodes.hpp"
#include "MaterialNode.hpp"
#include "ShapeNodes.hpp"
#include "TransformShapeNodes.hpp"
#include "Basic3DNodes.hpp"
#include "Basic3DTransformationNodes.hpp"
#include "TransformationNodes.hpp"
#include "BooleanNodes.hpp"
#include "ExpressionNode.hpp"
#include "PrismNode.hpp"

static NUIE::NodeRegistry nodeRegistry;
static bool initialized = false;

const NUIE::NodeRegistry& GetNodeRegistry ()
{
	if (!initialized) {
		nodeRegistry.RegisterNode (L"Input Nodes", L"Boolean",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::BooleanNode (L"Boolean", position, true));}
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"Integer",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", position, 0, 1));}
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"Number",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", position, 0.0, 1.0)); }
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"Integer Increment",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::IntegerIncrementedNode (L"Int Icrement", position)); }
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"Number Increment",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleIncrementedNode (L"Num Increment", position)); }
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"Number Distribute",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleDistributedNode (L"Num Distribute", position)); }
		);
		nodeRegistry.RegisterNode (L"Input Nodes", L"List Builder",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::ListBuilderNode (L"List Builder", position)); }
		);
		nodeRegistry.RegisterNode (L"Arithmetic Nodes", L"Addition",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", position)); }
		);
		nodeRegistry.RegisterNode (L"Arithmetic Nodes", L"Subtraction",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", position)); }
		);
		nodeRegistry.RegisterNode (L"Arithmetic Nodes", L"Multiplication",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", position)); }
		);
		nodeRegistry.RegisterNode (L"Arithmetic Nodes", L"Division",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DivisionNode (L"Division", position)); }
		);
		nodeRegistry.RegisterNode (L"Arithmetic Nodes", L"Expression",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ExpressionNode (L"Expression", position)); }
		);
		nodeRegistry.RegisterNode (L"Point Nodes", L"Point 2D",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new Point2DNode (L"Point 2D", position)); }
		);
		nodeRegistry.RegisterNode (L"Point Nodes", L"Point",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PointNode (L"Point", position)); }
		);
		nodeRegistry.RegisterNode (L"Point Nodes", L"Vector",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new VectorNode (L"Vector", position)); }
		);
		nodeRegistry.RegisterNode (L"Point Nodes", L"Line Points",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new LinePointsNode (L"Line Points", position)); }
		);
		nodeRegistry.RegisterNode (L"Point Nodes", L"Arc Points",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ArcPointsNode (L"Arc Points", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Material",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new MaterialNode (L"Material", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Box",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BoxNode (L"Box", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Box Shell",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BoxShellNode (L"Box Shell", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Cylinder",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new CylinderNode (L"Cylinder", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Cylinder Shell",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new CylinderShellNode (L"Cylinder Shell", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Cone",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ConeNode (L"Cone", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Sphere",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new SphereNode (L"Sphere", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Torus",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TorusNode (L"Torus", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Prism",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PrismNode (L"Prism", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Prism Shell",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PrismShellNode (L"Prism Shell", position)); }
		);
		nodeRegistry.RegisterNode (L"Shape Nodes", L"Platonic",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PlatonicNode (L"Platonic", position)); }
		);
		nodeRegistry.RegisterNode (L"Matrix Nodes", L"Translation Matrix",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TranslationMatrixNode (L"Translation Matrix", position)); }
		);
		nodeRegistry.RegisterNode (L"Matrix Nodes", L"Translation Matrix XYZ",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TranslationMatrixXYZNode (L"Translation Matrix XYZ", position)); }
		);
		nodeRegistry.RegisterNode (L"Matrix Nodes", L"Rotation Matrix",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new RotationMatrixNode (L"Rotation Matrix", position)); }
		);
		nodeRegistry.RegisterNode (L"Matrix Nodes", L"Scale Matrix",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ScaleMatrixNode (L"Scale Matrix", position)); }
		);
		nodeRegistry.RegisterNode (L"Matrix Nodes", L"Matrix Combination",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new MatrixCombinationNode (L"Matrix Combination", position)); }
		);
		nodeRegistry.RegisterNode (L"Transformation Nodes", L"Transform Point",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TransformPointNode (L"Transform Point", position)); }
		);
		nodeRegistry.RegisterNode (L"Transformation Nodes", L"Translate Shape",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TranslateShapeNode (L"Translate Shape", position)); }
		);
		nodeRegistry.RegisterNode (L"Transformation Nodes", L"Translate Shape XYZ",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TranslateShapeXYZNode (L"Translate Shape XYZ", position)); }
		);
		nodeRegistry.RegisterNode (L"Transformation Nodes", L"Rotate Shape",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new RotateShapeNode (L"Rotate Shape", position)); }
		);
		nodeRegistry.RegisterNode (L"Transformation Nodes", L"Transform Shape",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TransformShapeNode (L"Transform Shape", position)); }
		);
		nodeRegistry.RegisterNode (L"Boolean Nodes", L"Difference",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BooleanNode (L"Difference", position, BooleanNode::Operation::Difference)); }
		);
		nodeRegistry.RegisterNode (L"Boolean Nodes", L"Intersection",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BooleanNode (L"Intersection", position, BooleanNode::Operation::Intersection)); }
		);
		nodeRegistry.RegisterNode (L"Boolean Nodes", L"Union",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new UnionNode (L"Union", position)); }
		);
		nodeRegistry.RegisterNode (L"Other Nodes", L"Viewer",
			[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", position, 5)); }
		);
		initialized = true;
	}
	return nodeRegistry;
}
