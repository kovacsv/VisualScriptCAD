#include "UserData.hpp"

namespace Modeler
{

UserData::UserData ()
{
}

UserData::~UserData ()
{
}

UserDataCollection::UserDataCollection () :
	userDataCollection ()
{
}

void UserDataCollection::Set (const std::string& key, const UserDataConstPtr& data)
{
	if (userDataCollection.find (key) != userDataCollection.end ()) {
		userDataCollection.erase (key);
	}
	userDataCollection.insert ({ key, data });
}

UserDataConstPtr UserDataCollection::Get (const std::string& key) const
{
	auto found = userDataCollection.find (key);
	if (found == userDataCollection.end ()) {
		return nullptr;
	}
	return found->second;
}

}
