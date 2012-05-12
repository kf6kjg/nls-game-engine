/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all objects that need to implment materials.
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
	class MaterialInterface;
}

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class MaterialsSystemInterface {
	public:
		virtual ~MaterialsSystemInterface() {}
		
		virtual bool InsertMaterial(unsigned int, EnvelopeSPTR) = 0;
		virtual bool RemoveMaterial(unsigned int) = 0;
		
	protected:
		static MaterialInterface* CreateMaterial(EnvelopeSPTR);
		static void DestroyMaterial(MaterialInterface*);
		
	private:
		MaterialsSystemInterface& operator=(const MaterialsSystemInterface&); ///< Assignment operator disabled for pure interface class
		
	private:
		
	};
}
