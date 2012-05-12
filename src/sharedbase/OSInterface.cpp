/**
 * \file
 * \author Ricky Curtice
 * \date 2012-03-05
 * \brief Base interface for any OS specific code
*/
#include "OSInterface.h"

// Standard Includes

// Library Includes

// Local Includes

// Forward Declarations

// Typedefs

// Static data
OSInterfaceSPTR OSInterface::operatingSystem(nullptr);

// Static methods
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void OSInterface::SetOS(OSInterfaceSPTR operating_system) {
	if (OSInterface::operatingSystem.get() == nullptr) {
		OSInterfaceSPTR os(operating_system);
		OSInterface::operatingSystem = os;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
OSInterfaceSPTR OSInterface::GetOS() {
	OSInterfaceSPTR os(OSInterface::operatingSystem);
	return os;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool OSInterface::HasOS() {
	return OSInterface::operatingSystem.get() != nullptr;
}
