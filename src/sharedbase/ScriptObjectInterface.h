/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all objects that are to be made available to scripting.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes
#include <threading.h>

// Local Includes

// Forward Declarations

// Typedefs

// Local Constants

// Classes
class ScriptObjectInterface {
public:
	ScriptObjectInterface(unsigned int ref_count = 1) {
		this->refCount = ref_count;
	}
	
	ScriptObjectInterface(const ScriptObjectInterface& script_object) {
		this->refCount = 1; // New copies have little to do with the old copy in this case.
	}
	
	virtual ~ScriptObjectInterface() {}
	
	/// Angelscript reference count increment
	void Addref(void) {
		Threading::WriteLock w_lock(this->mutex);
		
		// Increase the reference counter
		this->refCount++;
	}
	
	/// Angelscript reference count decrement
	void Release(void) {
		// Decrease ref count 
		{
			Threading::WriteLock w_lock(this->mutex);
			--this->refCount;
		}
		
		// Delete if it reaches 0
		if (this->refCount <= 0) {
			delete this;
		}
	}
	
	/// Angelscript reference count readout
	unsigned int GetReferenceCount(void) {
		Threading::ReadLock r_lock(this->mutex);
		
		return refCount;
	}
	
private:
	ScriptObjectInterface& operator=(const ScriptObjectInterface&); ///< Assignment operator disabled for pure interface class
	
private:
	mutable Threading::ReadWriteMutex mutex;
	unsigned int refCount; ///< Number of script references in place.  When this reaches 0 the object is garbage collected.
};
