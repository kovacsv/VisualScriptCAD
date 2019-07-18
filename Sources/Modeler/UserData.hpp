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

class UserDataCollection
{
public:
	UserDataCollection ();

	void						Set (const std::string& key, const std::shared_ptr<UserData>& data);
	std::shared_ptr<UserData>	Get (const std::string& key) const;

	template <typename UserDataType>
	std::shared_ptr<UserDataType> GetTyped (const std::string& key) const;

private:
	std::unordered_map<std::string, std::shared_ptr<UserData>> userDataCollection;
};

template <typename UserDataType>
std::shared_ptr<UserDataType> UserDataCollection::GetTyped (const std::string& key) const
{
	return std::dynamic_pointer_cast<UserDataType> (Get (key));
}

}

#endif
