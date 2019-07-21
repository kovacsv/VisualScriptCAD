#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <string>
#include <memory>
#include <unordered_map>

namespace Modeler
{

class UserData
{
public:
	UserData ();
	virtual ~UserData ();
};

using UserDataPtr = std::shared_ptr<UserData>;
using UserDataConstPtr = std::shared_ptr<const UserData>;

class UserDataCollection
{
public:
	UserDataCollection ();

	void				Set (const std::string& key, const UserDataConstPtr& data);
	UserDataConstPtr	Get (const std::string& key) const;

	template <typename UserDataType>
	std::shared_ptr<const UserDataType> GetTyped (const std::string& key) const;

private:
	std::unordered_map<std::string, UserDataConstPtr> userDataCollection;
};

template <typename UserDataType>
std::shared_ptr<const UserDataType> UserDataCollection::GetTyped (const std::string& key) const
{
	return std::dynamic_pointer_cast<UserDataType> (Get (key));
}

}

#endif
