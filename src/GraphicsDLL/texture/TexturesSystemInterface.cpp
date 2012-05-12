/**
* \file
* \author Ricky Curtice
* \date 2012-01-16
* \brief Provides a common interface for all objects that need to implment textures.
*
* *TODO: A full description of the purpose of the file.
*
*/

#include "TexturesSystemInterface.h"

// Standard Includes
#include <cassert>
#include <set>

// Library Includes
#include <boost/lexical_cast.hpp>

// Local Includes
#include "../../sharedbase/Envelope.h"
#include "../Textures.h"

#include "TextureInterface.h"
#include "BasicTexture.h"
#include "TextureScroller.h"
#include "TextureAdjustable.h"

// Forward Declarations
namespace GraphicsCore {
	std::string getBasicTextureFileInfo(EnvelopeSPTR);
	void getScrollTextureInfo(EnvelopeSPTR, std::string&, float&, float&, float&);
	void getAdjustableTextureInfo(EnvelopeSPTR, std::string&, unsigned int&);
}

// Typedefs
namespace GraphicsCore {
	typedef std::set<TextureInterface*> TexSet;
}

// Local Constants

// Class Methods
namespace GraphicsCore {
	class InterfaceUpdateable;

	static TexSet gTextures;
	static RenderModule* gRenderModule;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	TextureInterface* TexturesSystemInterface::CreateTexture(EnvelopeSPTR envelope, RenderModule* render_module) {
		gRenderModule = render_module;
		TextureInterface* texture = nullptr;
		
		switch (envelope->msgid) {
			case ID_TEXTURE_BASIC: {
				std::string path = getBasicTextureFileInfo(envelope);
				
				if (path.length()) {
					texture = new BasicTexture(render_module, path);
				}
				else {
					texture = new BasicTexture();
				}
			}
			break;
			case ID_TEXTURE_SCROLL_ANIM: {
				TextureScroller* scoller;
				std::string path;
				float u_vel, v_vel, angle_vel;
				
				u_vel = v_vel = angle_vel = 0.0f;
				
				getScrollTextureInfo(envelope, path, u_vel, v_vel, angle_vel);
				
				if (path.length()) {
					scoller = new TextureScroller(new BasicTexture(render_module, path));
				}
				else {
					scoller = new TextureScroller();
				}
				
				scoller->SetUVVelocity(u_vel, v_vel);
				scoller->SetAngularVelocity(angle_vel);
				
				render_module->RegisterUpdateable(scoller);
				
				texture = scoller;
			}
			break;
			case ID_TEXTURE_ADJUSTABLE: {
				TextureAdjustable* newTexture;
				std::string path;
				unsigned int message_type_id = 0;
				
				getAdjustableTextureInfo(envelope, path, message_type_id);
				
				if (path.length()) {
					newTexture = new TextureAdjustable(new BasicTexture(render_module, path), render_module->GetMessageRouter(), render_module);
					newTexture->SetMessageId(message_type_id);
					
					texture = newTexture;
				}
				else {
					//*TODO: error
				}
			}
			break;
			default: {
				LOG(LOG_PRIORITY::INFO, "Unknown texture type: " + boost::lexical_cast < std::string > (envelope->msgid));
			}
		}
		
		// Make sure that no nulls ever get put in the textures list.
		if (texture != nullptr) {
			gTextures.insert(texture);
		}
		
		return texture;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TexturesSystemInterface::DestroyTexture(TextureInterface* texture) {
		// Only delete if it had been created and has not already been deleted.
		TexSet::iterator texit = gTextures.find(texture);
		if (texit != gTextures.end()) {
			assert(texture != nullptr); // Just be very sure.  It should be absolutely impossible to have a nullptr stored in the textures list.
			
			// Detect if it is an updateable object, and have it unregistered if it is.
			{
				InterfaceUpdateable* updateable = dynamic_cast<InterfaceUpdateable*>(texture);
				if (updateable != nullptr) {
					gRenderModule->UnregisterUpdateable(updateable);
				}
			}
			
			delete texture;
			
			gTextures.erase(texit);
		}
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Utility functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	std::string getBasicTextureFileInfo(EnvelopeSPTR envelope) {
		std::string path_out;
		unsigned int param_type;
		
		bool safe = true;
		
		unsigned int count = envelope->GetCount();
		for (unsigned int index = 0; index < count && safe; ++index) {
			param_type = envelope->GetDataUInt(index);
			switch (param_type) {
				case ID_TEXTURE_PARAM_FILE: {
					// Get the values
					path_out = envelope->GetDataString(index + 1);
					
					// Consume the positions used.
					index += 1;
				}
				break;
				default: {
					LOG(LOG_PRIORITY::INFO, "Unknown texture parameter type sent: " + boost::lexical_cast < std::string > (param_type));
					safe = false; // Stop processing the loop: to proceed would be just garbage due to unknown types.
				}
			}
		}
		
		return path_out;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void getScrollTextureInfo(EnvelopeSPTR envelope, std::string& path_out, float& u_vel_out, float& v_vel_out, float& angle_vel_out) {
		unsigned int param_type;
		
		bool safe = true;
		
		unsigned int count = envelope->GetCount();
		for (unsigned int index = 0; index < count && safe; ++index) {
			param_type = envelope->GetDataUInt(index);
			switch (param_type) {
				case ID_TEXTURE_PARAM_FILE: {
					// Get the values
					path_out = envelope->GetDataString(index + 1);
					
					// Consume the positions used.
					index += 1;
				}
				break;
				case ID_TEXTURE_PARAM_UV_VEL: {
					// Get the values
					u_vel_out = envelope->GetDataFloat(index + 1);
					v_vel_out = envelope->GetDataFloat(index + 2);
					
					// Consume the positions used.
					index += 2;
				}
				break;
				case ID_TEXTURE_PARAM_A_VEL: {
					// Get the values
					angle_vel_out = envelope->GetDataFloat(index + 1);
					
					// Consume the positions used.
					index += 1;
				}
				break;
				default: {
					LOG(LOG_PRIORITY::INFO, "Unknown texture parameter type sent: " + boost::lexical_cast < std::string > (param_type));
					safe = false; // Stop processing the loop: to proceed would be just garbage due to unknown types.
				}
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void getAdjustableTextureInfo(EnvelopeSPTR envelope, std::string& path_out, unsigned int& message_type_id_out) {
		unsigned int param_type;
		
		bool safe = true;
		
		unsigned int count = envelope->GetCount();
		for (unsigned int index = 0; index < count && safe; ++index) {
			param_type = envelope->GetDataUInt(index);
			switch (param_type) {
				case ID_TEXTURE_PARAM_FILE: {
					// Get the values
					path_out = envelope->GetDataString(index + 1);
					
					// Consume the positions used.
					index += 1;
				}
				break;
				case ID_TEXTURE_PARAM_MESSAGE_TYPE: {
					// Get the values
					message_type_id_out = envelope->GetDataUInt(index + 1);
					
					// Consume the positions used.
					index += 1;
				}
				break;
				default: {
					LOG(LOG_PRIORITY::INFO, "Unknown texture parameter type sent: " + boost::lexical_cast < std::string > (param_type));
					safe = false; // Stop processing the loop: to proceed would be just garbage due to unknown types.
				}
			}
		}
	}
	
}
