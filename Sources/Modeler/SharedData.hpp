#ifndef MODELER_SHAREDDATA_HPP
#define MODELER_SHAREDDATA_HPP

#include "Checksum.hpp"

#include <unordered_map>

namespace Modeler
{

template <typename IdType, typename DataType>
class SharedData
{
public:
	SharedData () :
		nextDataId (0)
	{

	}

	const DataType& GetData (IdType id) const
	{
		return idToData.at (id);
	}

	size_t DataCount () const
	{
		return idToData.size ();
	}

	IdType AddReference (const DataType& data, const Checksum& checksum)
	{
		IdType id = -1;
		auto foundData = checkSumToId.find (checksum);
		if (foundData == checkSumToId.end ()) {
			id = nextDataId++;
			idToData.insert ({ id, data });
			dataRefCount.insert ({ id, 1 });
			checkSumToId.insert ({ checksum, id });
			idToChecksum.insert ({ id, checksum });
		} else {
			id = foundData->second;
			dataRefCount[id] += 1;
		}
		return id;
	}

	void RemoveReference (IdType id)
	{
		dataRefCount[id] -= 1;
		if (dataRefCount[id] == 0) {
			idToData.erase (id);
			dataRefCount.erase (id);
			checkSumToId.erase (idToChecksum.at (id));
			idToChecksum.erase (id);
		}
	}

	void Clear ()
	{
		idToData.clear ();
		dataRefCount.clear ();
		checkSumToId.clear ();
		idToChecksum.clear ();
		nextDataId = 0;
	}

private:
	std::unordered_map<IdType, DataType>	idToData;
	std::unordered_map<IdType, int>			dataRefCount;
	std::unordered_map<Checksum, IdType>	checkSumToId;
	std::unordered_map<IdType, Checksum>	idToChecksum;

	IdType									nextDataId;
};

}

#endif
