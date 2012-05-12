/**
 * \file
 * \author Adam Martin
 * \date July 20 2011
 * \brief CoreInterface class used as a common base for all cores.
*/
#pragma once

// Standard Includes
#include <string>
#include <map>

// Library Includes
#include <boost/any.hpp>
#include <boost/function.hpp>

// Local Includes
#include "Entity_fwd.h"
#include "EventLogger.h"

// Forward Declarations
class PropertyMap;
class ComponentInterface;
class MessageRouter;
class Envelope;
class EntityList;

// Typedefs

class ModuleInterface { // All systems derive from this
public:
	ModuleInterface(PropertyMap* gprops, MessageRouter* msgrouter, EntityList* elist = nullptr) : msgrouter(msgrouter), gprops(gprops), elist(elist) { };
	virtual ~ModuleInterface(void) { }

	virtual void Update(double dt) = 0; // Called each game update with change in time (dt) in milliseconds since last update

	virtual void CreateComponent(std::string, std::map< std::string, boost::any > &, EntitySPTR) = 0; // Called to create a component of type (type), with the set of attributes (attributes), and with parent entity (e)

	virtual WHO_DELETES::TYPE RemoveComponent(ComponentInterface*) = 0;

protected:
	MessageRouter* msgrouter;
	PropertyMap* gprops;
	EntityList* elist;
};
