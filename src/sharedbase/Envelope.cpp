/**
 * \file
 * \author Adam Martin
 * \date July 21 2011
 * \brief Container for message data passed between cores.
*/

#include "Envelope.h"

// Standard Includes

// Library Includes
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <EngineConfig.h>

// Local Includes
#include "EventLogger.h"
#include "Entity.h"

// Local Consts
const std::string SERIALIZATION_ROOT("NLS_SD_1_0_0");

// Static class member initialization

// Class methods in the order they are defined within the class header

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SaveToDisk(const EnvelopeSPTR& envelope, const std::string& file) {
	boost::property_tree::ptree property_tree;
	std::string filename = file;
	
	LOG(LOG_PRIORITY::INFO, "Saving to disk in '" + filename + "'...");
	
	envelope->SaveToPropertyTree(property_tree, SERIALIZATION_ROOT + "");
	
	write_json(filename, property_tree);
	
	LOG(LOG_PRIORITY::INFO, "Completed saving to '" + filename + "'.");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool LoadFromDisk(const EnvelopeSPTR& envelope, const std::string& file) {
	std::string filename = file;
	
	// Prevent loading if there is any member data already.
	if (envelope->GetCount() > 0) {
		LOG(LOG_PRIORITY::INFO, "Cannot load from '" + filename + "' because the Envelope already contains data.");
		return false;
	}
	
	boost::property_tree::ptree property_tree;
	
	LOG(LOG_PRIORITY::INFO, "Loading from'" + filename + "'...");
	
	try {
		read_json(filename, property_tree);
	}
	catch (boost::property_tree::file_parser_error& exception) {
		LOG(LOG_PRIORITY::INFO, "Error '" + exception.message() + "' trying to read file: " + exception.filename());
		return false;
	}
	
	envelope->LoadFromPropertyTree(property_tree, SERIALIZATION_ROOT + "");
	// *BUG: *TODO: catch any bad parsing errors from malformed files, etc.
	
	LOG(LOG_PRIORITY::INFO, "Completed loading from'" + filename + "'.");
	return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Envelope::Envelope() :
	msgid(0)
	{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boost::any Envelope::GetData(const unsigned int& index) {
	Threading::ReadLock r_lock(this->mutex);
	
	return this->data.at(index).data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boost::any Envelope::GetData(const unsigned int& index) const {
	Threading::ReadLock r_lock(this->mutex);
	
	return this->data.at(index).data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool           Envelope::GetDataBool  (const unsigned int& index) { return this->GetDataValue<bool>          (index); }
int            Envelope::GetDataInt   (const unsigned int& index) { return this->GetDataValue<int>           (index); }
long           Envelope::GetDataLong  (const unsigned int& index) { return this->GetDataValue<long>          (index); }
unsigned int   Envelope::GetDataUInt  (const unsigned int& index) { return this->GetDataValue<unsigned int>  (index); }
float          Envelope::GetDataFloat (const unsigned int& index) { return this->GetDataValue<float>         (index); }
std::string    Envelope::GetDataString(const unsigned int& index) { return this->GetDataValue<std::string>   (index); }
glm::vec3      Envelope::GetDataVector(const unsigned int& index) { return this->GetDataValue<glm::vec3>     (index); }
glm::fquat     Envelope::GetDataQuat  (const unsigned int& index) { return this->GetDataValue<glm::fquat>    (index); }
glm::vec4      Envelope::GetDataColor (const unsigned int& index) { return this->GetDataValue<glm::vec4>     (index); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EntitySPTR   Envelope::GetDataEntityP     (const unsigned int& index) { EntitySPTR   sptr(this->GetDataReference<EntitySPTR>(index));   return sptr; }
Envelope*    Envelope::GetDataEnvelopeP   (const unsigned int& index) { Envelope*     ptr(this->GetDataReference<Envelope*> (index));   return  ptr; }
EnvelopeSPTR Envelope::GetDataEnvelopeSPTR(const unsigned int& index) { EnvelopeSPTR sptr(this->GetDataReference<EnvelopeSPTR>(index)); return sptr; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned int Envelope::GetCount() {
	Threading::ReadLock r_lock(this->mutex); 
	
	return this->data.size();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Envelope::SaveToPropertyTree(boost::property_tree::ptree& property_tree, const std::string& parent_key) {
	Threading::ReadLock r_lock(this->mutex);
	
	LOG(LOG_PRIORITY::INFO, "Saving Envelope(" + boost::lexical_cast<std::string>(this->msgid) + ")'s data to property map...");
	
	// Store the message id
	property_tree.put(parent_key + ".message_id", this->msgid);
	
	unsigned int counter = 0;
	
	// Store the data
	BOOST_FOREACH(EnvelopeItem datum, this->data) {
		if (datum.data.type() == typeid(bool)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + "." + datum.type, boost::any_cast<bool>(datum.data));
		}
		else if (datum.data.type() == typeid(int)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + "." + datum.type, boost::any_cast<int>(datum.data));
		}
		else if (datum.data.type() == typeid(long)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + "." + datum.type, boost::any_cast<long>(datum.data));
		}
		else if (datum.data.type() == typeid(unsigned int)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + "." + datum.type, boost::any_cast<unsigned int>(datum.data));
		}
		else if (datum.data.type() == typeid(float)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + "." + datum.type, boost::any_cast<float>(datum.data));
		}
		else if (datum.data.type() == typeid(std::string)) {
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".string", boost::any_cast<std::string>(datum.data));
		}
		else if (datum.data.type() == typeid(glm::vec3)) {
			glm::vec3 vector = boost::any_cast<glm::vec3>(datum.data);
			
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".vector3.x", vector.x);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".vector3.y", vector.y);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".vector3.z", vector.z);
		}
		else if (datum.data.type() == typeid(glm::fquat)) {
			glm::fquat quat = boost::any_cast<glm::fquat>(datum.data);
			
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".quat.x", quat.x);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".quat.y", quat.y);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".quat.z", quat.z);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".quat.w", quat.w);
		}
		else if (datum.data.type() == typeid(glm::vec4)) {
			glm::vec4 color = boost::any_cast<glm::vec4>(datum.data);
			
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".r", color.r);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".g", color.g);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".b", color.b);
			property_tree.put(parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".a", color.a);
		}
		else if (datum.data.type() == typeid(Envelope*)) {
			LOG(LOG_PRIORITY::INFO, "Serializing an Envelope pointer unsupported at this time - please use an EnvelopeSPTR.");
		}
		else if (datum.data.type() == typeid(EnvelopeSPTR)) {
			boost::any_cast<EnvelopeSPTR>(datum.data)->SaveToPropertyTree(property_tree, parent_key + ".data.item" + boost::lexical_cast<std::string>(counter) + ".envelope");
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Serializing an " + std::string(datum.data.type().name()) + " unsupported at this time.");
		}
		counter++;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Envelope::LoadFromPropertyTree(boost::property_tree::ptree& property_tree, const std::string& parent_key) {
	LOG(LOG_PRIORITY::INFO, "Loading envelope data...");
	
	BOOST_FOREACH(boost::property_tree::ptree::value_type& value, property_tree.get_child(parent_key)) {
		if (value.first == "message_id") {
			this->msgid = boost::lexical_cast<int>(value.second.data());
		}
		else if (value.first == "data") {
			boost::property_tree::ptree data = value.second;
			
			BOOST_FOREACH(boost::property_tree::ptree::value_type& value, data) {
				boost::property_tree::ptree datum = value.second;
				
				boost::property_tree::ptree::iterator it;
				if ((it = datum.to_iterator(datum.find("bool"))) != datum.end()) {
					this->AddData(boost::lexical_cast<bool>(it->second.data()));
				}
				else if ((it = datum.to_iterator(datum.find("int"))) != datum.end()) {
					this->AddData(boost::lexical_cast<int>(it->second.data()));
				}
				else if ((it = datum.to_iterator(datum.find("long"))) != datum.end()) {
					this->AddData(boost::lexical_cast<long>(it->second.data()));
				}
				else if ((it = datum.to_iterator(datum.find("unsigned int"))) != datum.end()) {
					this->AddData(boost::lexical_cast<unsigned int>(it->second.data()));
				}
				else if ((it = datum.to_iterator(datum.find("float"))) != datum.end()) {
					this->AddData(boost::lexical_cast<float>(it->second.data()));
				}
				else if ((it = datum.to_iterator(datum.find("string"))) != datum.end()) {
					this->AddData(it->second.data());
				}
				else if ((it = datum.to_iterator(datum.find("vector3"))) != datum.end()) {
					glm::vec3 vector3;
					vector3.x = it->second.get<float>("x");
					vector3.y = it->second.get<float>("y");
					vector3.z = it->second.get<float>("z");
					
					this->AddData(vector3);
				}
				else if ((it = datum.to_iterator(datum.find("quat"))) != datum.end()) {
					glm::fquat quat;
					quat.x = it->second.get<float>("x");
					quat.y = it->second.get<float>("y");
					quat.z = it->second.get<float>("z");
					quat.w = it->second.get<float>("w");
					
					this->AddData(quat);
				}
				else if ((it = datum.to_iterator(datum.find("color"))) != datum.end()) {
					glm::vec4 color;
					color.r = it->second.get<float>("r");
					color.g = it->second.get<float>("g");
					color.b = it->second.get<float>("b");
					color.a = it->second.get<float>("a");
					
					this->AddData(color);
				}
				else if ((it = datum.to_iterator(datum.find("envelope"))) != datum.end()) {
					EnvelopeSPTR envelope(new Envelope());
					
					envelope->LoadFromPropertyTree(it->second, "");
					
					this->AddData(envelope);
				}
				else  {
					LOG(LOG_PRIORITY::INFO, "Deserializing whatever's in the '" + value.first + "' unsupported at this time.");
				}
			}
		}
		else  {
			LOG(LOG_PRIORITY::INFO, "Deserializing a '" + value.first + "' data item unsupported at this time.");
		}
	}
	
}
