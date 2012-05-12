/**
 * \file
 * \author Ricky Curtice
 * \date 2012-01-04
 * \brief Defines the class methods relating to the materials system for the render engine module.
 *
 * <p>
 * </p>
 *
 */

#include "RenderModule.h"

// Standard Includes
#include <string>
#include <deque>

// Library Includes

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/Envelope.h"

#include "material/MaterialsSystemInterface.h"

#include "Messages.h"

// Typedefs and structs

// Constants

// Class Mathods
namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::CreateMaterial(EnvelopeSPTR incomingEnvelope, MaterialsSystemInterface* object) const {
		const static unsigned int MATERIAL_MESSAGE_STRIDE = 2;
		
		int index = incomingEnvelope->GetCount();
		
		// If there's a material's list, it should at least have some members!
		if (index >= MATERIAL_MESSAGE_STRIDE) {
			unsigned int material_index;
			EnvelopeSPTR material_spec;
			
			bool status = true;
			
			while ((index -= MATERIAL_MESSAGE_STRIDE) >= 0 && status) {
				material_index = incomingEnvelope->GetDataUInt(        index + 0);
				material_spec  = incomingEnvelope->GetDataEnvelopeSPTR(index + 1);
				
				status = object->InsertMaterial(material_index, material_spec);
			}
			
			return status;
		}
		
		LOG(LOG_PRIORITY::INFO, "Attempted creation of a materials list without a material!");
		
		return false;
	}
}
