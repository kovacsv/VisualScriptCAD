#include "ReadWrite.hpp"

NE::Stream::Status ReadVector (NE::InputStream& inputStream, glm::vec3& val)
{
	for (glm::vec3::length_type i = 0; i < 3; i++) {
		inputStream.Read (val[i]);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status WriteVector (NE::OutputStream& outputStream, const glm::vec3& val)
{
	for (glm::vec3::length_type i = 0; i < 3; i++) {
		outputStream.Write (val[i]);
	}
	return outputStream.GetStatus ();
}

NE::Stream::Status ReadMatrix (NE::InputStream& inputStream, glm::mat4& val)
{
	for (glm::vec3::length_type i = 0; i < 4; i++) {
		for (glm::vec3::length_type j = 0; j < 4; j++) {
			inputStream.Read (val[i][j]);
		}
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status WriteMatrix (NE::OutputStream& outputStream, const glm::mat4& val)
{
	for (glm::vec3::length_type i = 0; i < 4; i++) {
		for (glm::vec3::length_type j = 0; j < 4; j++) {
			outputStream.Write (val[i][j]);
		}
	}
	return outputStream.GetStatus ();
}

NE::Stream::Status ReadVector (NE::InputStream& inputStream, glm::dvec3& val)
{
	for (glm::dvec3::length_type i = 0; i < 3; i++) {
		inputStream.Read (val[i]);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status WriteVector (NE::OutputStream& outputStream, const glm::dvec3& val)
{
	for (glm::dvec3::length_type i = 0; i < 3; i++) {
		outputStream.Write (val[i]);
	}
	return outputStream.GetStatus ();
}

NE::Stream::Status ReadMatrix (NE::InputStream& inputStream, glm::dmat4& val)
{
	for (glm::dvec3::length_type i = 0; i < 4; i++) {
		for (glm::dvec3::length_type j = 0; j < 4; j++) {
			inputStream.Read (val[i][j]);
		}
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status WriteMatrix (NE::OutputStream& outputStream, const glm::dmat4& val)
{
	for (glm::dvec3::length_type i = 0; i < 4; i++) {
		for (glm::dvec3::length_type j = 0; j < 4; j++) {
			outputStream.Write (val[i][j]);
		}
	}
	return outputStream.GetStatus ();
}
