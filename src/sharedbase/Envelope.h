/**
* \file
* \author Adam Martin
* \date 2011-07-21
* \brief Container for message data passed between cores.
*
*/
#pragma once

// Standard Includes
#include <vector>

// Library Includes
#include <boost/any.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Local Includes
#include "EventLogger.h"
#include "Envelope_fwd.h"
#include "Entity_fwd.h"

// Forward Declarations

/// Saves the EnvelopeSPTR, including all its data, into the named file.
void SaveToDisk(const EnvelopeSPTR&, const std::string&);

/// Loads the EnvelopeSPTR, including all its data, from the named file.  Note that this will only work if GetCount returns 0 - ie: there's nothing stored in the envelope.
bool LoadFromDisk(const EnvelopeSPTR&, const std::string&);


// Typedefs

class Envelope {
public:
	Envelope();
	~Envelope() {}
	
	boost::any     GetData            (const unsigned int& = 0);///< Get the data stored at index i
	boost::any     GetData            (const unsigned int& = 0) const;///< Get the data stored at index i
	bool           GetDataBool        (const unsigned int& = 0);
	int            GetDataInt         (const unsigned int& = 0);
	long           GetDataLong        (const unsigned int& = 0);
	unsigned int   GetDataUInt        (const unsigned int& = 0);
	float          GetDataFloat       (const unsigned int& = 0);
	std::string    GetDataString      (const unsigned int& = 0);
	glm::vec3      GetDataVector      (const unsigned int& = 0);
	glm::fquat     GetDataQuat        (const unsigned int& = 0);
	glm::vec4      GetDataColor       (const unsigned int& = 0);
	EntitySPTR     GetDataEntityP     (const unsigned int& = 0);
	Envelope*      GetDataEnvelopeP   (const unsigned int& = 0);
	EnvelopeSPTR   GetDataEnvelopeSPTR(const unsigned int& = 0);
	
	template <typename T>
	T GetDataReference(const unsigned int& index) {
		Threading::ReadLock r_lock(this->mutex);
		
		boost::any a = (this->GetData(index));
		if (a.type() == typeid(T)) {
			return boost::any_cast<T>(a);
		}
		return nullptr;
	}
	
	template <typename T>
	T GetDataValue(const unsigned int& index) {
		Threading::ReadLock r_lock(this->mutex);
		
		boost::any a = (this->GetData(index));
		if (a.type() == typeid(T)) {
			return boost::any_cast<T>(a);
		}
		return T();
	}
	
	template<typename T>
	void AddData(const T& data) { ///< Adds more data to the envelope
		Threading::WriteLock w_lock(this->mutex);
		
		this->data.push_back(EnvelopeItem(typeid(data).name(), data));
	}
	
	template<typename T>
	void AddDataValue(const T& data) { ///< Adds more data to the envelope
		Threading::WriteLock w_lock(this->mutex);
		
		this->data.push_back(EnvelopeItem(typeid(data).name(), data));
	}
	
	template<typename T>
	void AddDataReference(T data) { ///< Adds more data to the envelope
		Threading::WriteLock w_lock(this->mutex);
		
		this->data.push_back(EnvelopeItem(typeid(data).name(), data));
	}
	
	/// Returns how many data elements exist in this envelope
	unsigned int GetCount();
	
	/// Called to serialize this object into a predefined location in a given property tree.
	void SaveToPropertyTree(boost::property_tree::ptree&, const std::string&);
	
	/// Called to unserialize this object from a predefined location in a given property tree.
	void LoadFromPropertyTree(boost::property_tree::ptree&, const std::string&);
	
public: // Public properties
	int msgid; // Used to identify the message type
	
private: // Utility methods
	template <typename T>
	T TGetData(unsigned int index);
	
private: // Private types
	struct EnvelopeItem {
		EnvelopeItem(const std::string& type, const boost::any& data) :
			type(type),
			data(data)
		{}
		
		std::string type;
		boost::any data;
	};
	
private: // Member Data
	mutable Threading::ReadWriteMutex mutex;
	std::vector<EnvelopeItem> data;
};
