#include "Basic3DNodeValues.hpp"
#include "NE_StringUtils.hpp"
#include "GLMReadWrite.hpp"

NE::SerializationInfo			CoordinateValue::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	Point2DValue::serializationInfo (NE::ObjectId ("{6AEC122C-5F57-4EDF-8911-3F6B1ABC445D}"), NE::ObjectVersion (1), Point2DValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{6A357CF8-F6BB-4760-962E-B1864DF157F7}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	VectorValue::serializationInfo (NE::ObjectId ("{2B24AB4D-AFD3-430C-817E-FE18A6A69A23}"), NE::ObjectVersion (1), VectorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformationValue::serializationInfo (NE::ObjectId ("{395C8CD0-EF53-4619-AB1A-DB31068DF50C}"), NE::ObjectVersion (1), TransformationValue::CreateSerializableInstance);

Point2DValue::Point2DValue () :
	Point2DValue (glm::vec2 (0.0))
{

}

Point2DValue::Point2DValue (const glm::vec2& val) :
	NE::GenericValue<glm::vec2> (val)
{

}

NE::ValuePtr Point2DValue::Clone () const
{
	return NE::ValuePtr (new Point2DValue (val));
}

std::wstring Point2DValue::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Point (";
	result += NE::DoubleToString (val.x, stringSettings);
	result += L", ";
	result += NE::DoubleToString (val.y, stringSettings);
	result += L")";
	return result;
}

NE::Stream::Status Point2DValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<glm::vec2>::Read (inputStream);
	ReadVector (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status Point2DValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<glm::vec2>::Write (outputStream);
	WriteVector (outputStream, val);
	return outputStream.GetStatus ();
}

CoordinateValue::CoordinateValue () :
	CoordinateValue (glm::vec3 (0.0))
{

}

CoordinateValue::CoordinateValue (const glm::vec3& val) :
	NE::GenericValue<glm::vec3> (val)
{

}

std::wstring CoordinateValue::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += GetNameString () + L" (";
	result += NE::DoubleToString (val.x, stringSettings);
	result += L", ";
	result += NE::DoubleToString (val.y, stringSettings);
	result += L", ";
	result += NE::DoubleToString (val.z, stringSettings);
	result += L")";
	return result;
}

NE::Stream::Status CoordinateValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<glm::vec3>::Read (inputStream);
	ReadVector (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status CoordinateValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<glm::vec3>::Write (outputStream);
	WriteVector (outputStream, val);
	return outputStream.GetStatus ();
}

PointValue::PointValue () :
	PointValue (glm::vec3 (0.0))
{

}

PointValue::PointValue (const glm::vec3& val) :
	CoordinateValue (val)
{

}

NE::ValuePtr PointValue::Clone () const
{
	return NE::ValuePtr (new PointValue (val));
}

NE::Stream::Status PointValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	CoordinateValue::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	CoordinateValue::Write (outputStream);
	return outputStream.GetStatus ();
}

std::wstring PointValue::GetNameString () const
{
	return L"Point";
}

VectorValue::VectorValue () :
	VectorValue (glm::vec3 (0.0))
{

}

VectorValue::VectorValue (const glm::vec3& val) :
	CoordinateValue (val)
{

}

NE::ValuePtr VectorValue::Clone () const
{
	return NE::ValuePtr (new VectorValue (val));
}

NE::Stream::Status VectorValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	CoordinateValue::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status VectorValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	CoordinateValue::Write (outputStream);
	return outputStream.GetStatus ();
}

std::wstring VectorValue::GetNameString () const
{
	return L"Vector";
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
