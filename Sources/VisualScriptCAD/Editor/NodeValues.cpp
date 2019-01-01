#include "NodeValues.hpp"
#include "NE_StringUtils.hpp"
#include "ReadWrite.hpp"

NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{6A357CF8-F6BB-4760-962E-B1864DF157F7}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformationValue::serializationInfo (NE::ObjectId ("{395C8CD0-EF53-4619-AB1A-DB31068DF50C}"), NE::ObjectVersion (1), TransformationValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	ShapeValue::serializationInfo (NE::ObjectId ("{3C6EA711-831F-4A16-AC74-6A43A1AB7ACD}"), NE::ObjectVersion (1), ShapeValue::CreateSerializableInstance);

PointValue::PointValue () :
	PointValue (glm::vec3 (0.0))
{

}

PointValue::PointValue (const glm::vec3& val) :
	NE::GenericValue<glm::vec3> (val)
{

}

NE::ValuePtr PointValue::Clone () const
{
	return NE::ValuePtr (new PointValue (val));
}

std::wstring PointValue::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Point (";
	result += NE::DoubleToString (val.x, stringSettings);
	result += L", ";
	result += NE::DoubleToString (val.y, stringSettings);
	result += L", ";
	result += NE::DoubleToString (val.z, stringSettings);
	result += L")";
	return result;
}

NE::Stream::Status PointValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<glm::vec3>::Read (inputStream);
	ReadVector (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<glm::vec3>::Write (outputStream);
	WriteVector (outputStream, val);
	return outputStream.GetStatus ();
}

TransformationValue::TransformationValue () :
	TransformationValue (glm::mat4 (1.0f))
{

}

TransformationValue::TransformationValue (const glm::mat4& val) :
	NE::GenericValue<glm::mat4> (val)
{

}

NE::ValuePtr TransformationValue::Clone () const
{
	return NE::ValuePtr (new TransformationValue (val));
}

std::wstring TransformationValue::ToString (const NE::StringSettings&) const
{
	// TODO
	std::wstring result;
	result += L"transformation";
	return result;
}

NE::Stream::Status TransformationValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<glm::mat4>::Read (inputStream);
	ReadMatrix (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformationValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<glm::mat4>::Write (outputStream);
	WriteMatrix (outputStream, val);
	return outputStream.GetStatus ();
}

ShapeValue::ShapeValue () :
	ShapeValue (nullptr)
{

}

ShapeValue::ShapeValue (const Modeler::ShapePtr& val) :
	NE::GenericValue<Modeler::ShapePtr> (val)
{

}

NE::ValuePtr ShapeValue::Clone () const
{
	return NE::ValuePtr (new ShapeValue (val->Clone ()));
}

std::wstring ShapeValue::ToString (const NE::StringSettings&) const
{
	return val->ToString ();
}

NE::Stream::Status ShapeValue::Read (NE::InputStream& inputStream)
{
	DBGBREAK ();
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Modeler::ShapePtr>::Read (inputStream);
	//val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ShapeValue::Write (NE::OutputStream& outputStream) const
{
	DBGBREAK ();
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Modeler::ShapePtr>::Write (outputStream);
	//val.Write (outputStream);
	return outputStream.GetStatus ();
}
