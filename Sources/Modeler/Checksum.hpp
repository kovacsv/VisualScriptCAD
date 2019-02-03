#ifndef CHECKSUM_HPP
#define CHECKSUM_HPP

#include <stdint.h>
#include <functional>

namespace Modeler
{

class Checksum
{
public:
	Checksum ();
	~Checksum ();

	void	Add (int val);
	void	Add (unsigned int val);
	void	Add (size_t val);
	void	Add (int64_t val);
	void	Add (float val);
	void	Add (double val);
	void	Add (const Checksum& val);

	bool	operator== (const Checksum& rhs) const;
	bool	operator!= (const Checksum& rhs) const;

	size_t	GenerateHashValue () const;

private:
	void	AddInteger (int val);
	
	int		checksum;
	int		counter;
};

}

namespace std
{
	template <>
	struct hash<Modeler::Checksum>
	{
		size_t operator() (const Modeler::Checksum& checksum) const noexcept
		{
			return checksum.GenerateHashValue ();
		}
	};
}

#endif
