#pragma once

/**
* \file
* \author Adam Martin
* \date 2011-07-25
* \brief Properties class to maintain global properties for use in the whole engine.
*
* A simple way to pass around properties and values between modules.
*
*/

// Standard Includes
#include <map>
#include <string>

// Library Includes
#include <boost/any.hpp>
#include <boost/unordered_map.hpp>
#include <threading.h>

// Local Includes

// Forward Declarations

// Typedefs

class PropertyMap {
public:
	PropertyMap(void) { }
	
	PropertyMap(const PropertyMap& orig) {
		Threading::ReadLock r_lock(orig.mutex);
		this->propmap = orig.propmap;
	}
	
	~PropertyMap(void) { }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template<typename T>
	void SetProperty(std::string key, T value) {
		Threading::WriteLock w_lock(this->mutex);
		
		this->propmap[key] = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool PropertyExists(std::string key) {
		Threading::ReadLock r_lock(this->mutex);
		
		if (this->propmap.find(key) != this->propmap.end()) {
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	boost::any GetProperty(std::string key) {
		Threading::ReadLock r_lock(this->mutex);
		
		if (this->propmap.find(key) != this->propmap.end()) {
			return this->propmap[key];
		}
		return boost::any();
	}

	// Template specializations for different return types
	template <typename t>
	t GetPropertyReference(std::string key) {
		Threading::ReadLock r_lock(this->mutex);
		
		boost::any a = (GetProperty(key));
		if (a.type() == typeid(t)) {
			return boost::any_cast<t>(a);
		}
		return nullptr;
	}

	template <typename t>
	t GetPropertyValue(std::string key) {
		Threading::ReadLock r_lock(this->mutex);
		
		boost::any a = (GetProperty(key));
		if (a.type() == typeid(t)) {
			return boost::any_cast<t>(a);
		}
		return t();
	}

	void swap(PropertyMap& rhs) {
		Threading::ReadLock r_lock(this->mutex);
		
		this->propmap.swap(rhs.propmap);
	}

	PropertyMap& operator=(const PropertyMap& rhs) {
		PropertyMap temp(rhs); // Copy the rhs,
		swap(temp); // swap the contents, viola, assignment done! (Got to love "Effective C++" 3rd Ed. by Scott Meyers.)
		
		return *this;
	}

private:
	mutable Threading::ReadWriteMutex mutex;
	std::map<std::string, boost::any> propmap;
};
