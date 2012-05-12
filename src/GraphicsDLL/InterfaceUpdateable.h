/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all objects that require updates.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes

// Local Includes

// Forward Declarations

// Typedefs

namespace GraphicsCore {
	class InterfaceUpdateable {
	public:
		virtual ~InterfaceUpdateable() {}
		
		virtual void Update(double = 0.0) = 0;
		
	private:
		InterfaceUpdateable& operator=(const InterfaceUpdateable&); ///< Assignment operator disabled for pure interface class
	};
}
