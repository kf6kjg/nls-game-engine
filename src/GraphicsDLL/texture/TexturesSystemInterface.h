/**
* \file
* \author Ricky Curtice
* \date 2012-01-16
* \brief Provides a common interface for all objects that need to implment textures.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes

// Local Includes
#include "../../sharedbase/Envelope_fwd.h"

// Forward Declarations
namespace GraphicsCore {
	class RenderModule;
	class TextureInterface;
}

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class TexturesSystemInterface {
		friend class TextureAdjustable;
		
	public:
		virtual ~TexturesSystemInterface() {}
		
		virtual bool InsertTexture(unsigned int, EnvelopeSPTR) = 0;
		virtual bool RemoveTexture(unsigned int) = 0;
		
	protected:
		static TextureInterface* CreateTexture(EnvelopeSPTR, RenderModule*);
		
	public:
		static void DestroyTexture(TextureInterface*);
		
	private:
		TexturesSystemInterface& operator=(const TexturesSystemInterface&); ///< Assignment operator disabled for pure interface class
		
	private:
		
	};
}
