#ifndef VERSION_HPP
#define VERSION_HPP

#include "NE_Stream.hpp"

class Version
{
public:
	Version ();
	Version (int version1, int version2, int version3);

	bool				operator< (const Version& rhs) const;
	bool				operator> (const Version& rhs) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	int version1;
	int version2;
	int version3;
};

extern const Version AppVersion;
extern const int FileVersion;

#endif
