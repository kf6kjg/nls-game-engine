/**
 * \file
 * \author Ricky Curtice
 * \date 2012-01-16
 * \brief Defines the class methods relating to the textures system for the render engine module.
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

#include "texture/TexturesSystemInterface.h"

#include "Messages.h"

// Typedefs and structs

// Constants

// Class Mathods
namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::CreateTexture(EnvelopeSPTR incomingEnvelope, TexturesSystemInterface* object) const {
		const static unsigned int TEXTURE_MESSAGE_STRIDE = 2;
		
		int index = incomingEnvelope->GetCount();
		
		// If there's a texture's list, it should at least have some members!
		if (index >= TEXTURE_MESSAGE_STRIDE) {
			unsigned int texture_index;
			EnvelopeSPTR texture_spec;
			
			bool status = true;
			
			while ((index -= TEXTURE_MESSAGE_STRIDE) >= 0 && status) {
				texture_index = incomingEnvelope->GetDataUInt(        index + 0);
				texture_spec  = incomingEnvelope->GetDataEnvelopeSPTR(index + 1);
				
				status = object->InsertTexture(texture_index, texture_spec);
			}
			
			return status;
		}
		
		LOG(LOG_PRIORITY::INFO, "Attempted creation of a textures list without a texture!");
		
		return false;
	}
}
