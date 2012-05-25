/**
 * \file
 * \author Ricky Curtice
 * \date 2012-03-05
 * \brief Base interface for any OS specific code
*/
#pragma once

// Standard Includes
#include <memory>

// Library Includes

// Local Includes

// Forward Declarations
class OSInterface;

// Typedefs
typedef std::shared_ptr<OSInterface> OSInterfaceSPTR;

// Enumerations

/// Enumeration of known paths.  This provides an OS abstraction.
namespace SYSTEM_DIRS {
	enum TYPE {
		USER, ///< On Linux: "$HOME". On Windows XP: SHGetFolderPath(CSIDL_APPDATA). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_RoamingAppData). on Mac: 
		DOCUMENTS, ///< On Linux: . On Windows XP: SHGetFolderPath(CSIDL_MYDOCUMENTS). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_Documents). on Mac: 
		PICTURES, ///< On Linux: . On Windows XP: SHGetFolderPath(CSIDL_MYPICTURES). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_Pictures). on Mac: 
		MUSIC, ///< On Linux: . On Windows XP: SHGetFolderPath(CSIDL_MYMUSIC). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_Music). on Mac: 
		VIDEO, ///< On Linux: . On Windows XP: SHGetFolderPath(CSIDL_MYVIDEO). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_Videos). on Mac: 
		DESKTOP, ///< On Linux: . On Windows XP: SHGetFolderPath(CSIDL_DESKTOPDIRECTORY). On Windows Vista (or newer): SHGetKnownFolderPath(FOLDERID_Desktop). on Mac: 
		EXECUTABLE, ///< Specifies the path to the directory containing the engine/game's executable.
		d1234 ///< nothing, just a placeholder. DO NOT USE.
	};
}

enum WINDOW_FLAGS {
	WINDOW_OUTER_SIZE = 0, ///< Window should be sized by the outer boundry.
	WINDOW_INNER_SIZE = 1, ///< Window should be sized by the inside ("client") boundry.
	WINDOW_1234 ///< nothing, just a placeholder. DO NOT USE.
};

