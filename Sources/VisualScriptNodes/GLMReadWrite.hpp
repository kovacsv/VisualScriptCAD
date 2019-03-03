#ifndef GLMREADWRITE_HPP
#define GLMREADWRITE_HPP

#include "NE_Stream.hpp"
#include "IncludeGLM.hpp"

NE::Stream::Status	ReadVector (NE::InputStream& inputStream, glm::vec2& val);
NE::Stream::Status	WriteVector (NE::OutputStream& outputStream, const glm::vec2& val);

NE::Stream::Status	ReadVector (NE::InputStream& inputStream, glm::vec3& val);
NE::Stream::Status	WriteVector (NE::OutputStream& outputStream, const glm::vec3& val);

NE::Stream::Status	ReadMatrix (NE::InputStream& inputStream, glm::mat4& val);
NE::Stream::Status	WriteMatrix (NE::OutputStream& outputStream, const glm::mat4& val);

NE::Stream::Status	ReadVector (NE::InputStream& inputStream, glm::dvec3& val);
NE::Stream::Status	WriteVector (NE::OutputStream& outputStream, const glm::dvec3& val);

NE::Stream::Status	ReadMatrix (NE::InputStream& inputStream, glm::dmat4& val);
NE::Stream::Status	WriteMatrix (NE::OutputStream& outputStream, const glm::dmat4& val);

#endif
