/**
* \file
* \author Adam Martin
* \date 2011-10-25
* \brief Simple event logger to output debug and other messages to file
*
*
*/
#pragma once


// Standard Includes
#include <queue>
#include <string>

// Library Includes
#include <threading.h>

// Local Includes

// Forward Declarations
class ScriptEngine;

// Typedefs

// Macro to make messages more meaningful
/// Logging macro designed to make logged messages much more meaningful by providing file, line number, etc.
#define LOG(x, y) ::EventLogger::GetEventLogger()->LogToDisk((x), (y), __FILE__, __LINE__, __FUNCTION__);

/// Namespaced enumerated type to specify the priority status of a logging message.
namespace LOG_PRIORITY {
	/// Enumerated type for logging.
	enum TYPE {
		INFO = 0, ///< Used to log general information.
		FLOW = 1, ///< Used to log program flow for debugging purposes.
		
		WARN = 2, ///< Used to log general warnings, usually about data that is not correctly formed, but wasn't a showstopper.
		
		CONFIG   = 3, ///< Used to log errors with the configuration - usually showstoppers, but not always; the code might be able to continue using a default value.
		ERR      = 4, ///< Used to log errors in operation - usually showstoppers, but not always.
		MISSRESS = 5, ///< Used to log that a requested resource doesn't exist: missing files, etc.  A default may be provided.
		RESTART  = 6, ///< Used to indicate that a critical core tool has crashed or is unable to start: tools like modules, threads, etc.
		SYSERR   = 7, ///< A critical system error or exception has occurred and the program is now closing.
		
		DEPRECATE = 8, ///< Used to log that something has been deprecated.
		
		ENUM_COUNT
	};
	
	/// String values to be printed in the for the log file for the above.
	static const std::string PRINTABLE_NOTICES[ENUM_COUNT] = {
		"INFORMATION", // 0
		"PROGRAMFLOW", // 1
		"WARNING    ", // 2
		"BADCONFIG  ", // 3
		"ERROR      ", // 4
		"MISSINGRESS", // 5
		"RESTART/SET", // 6
		"SYSTEMERROR", // 7
		"DEPRECATED " // 8
	};
}

class EventLogger {
public: // Public static members
	static void SetEventLogger(EventLogger*);
	static EventLogger* GetEventLogger();

	static void RegisterScriptEngine(ScriptEngine* const);
	
public: // Public static parameters
	static std::string module;
	
public: // Public members
	~EventLogger(void);
	
	/// Log the message to disk, with enough information to determine source.
	bool LogToDisk(const LOG_PRIORITY::TYPE&, const std::string&, const std::string& = "<NO FILE>", const unsigned int& = 0, const std::string& = "<NO FUNCTION>");
	
	/// Specify a log file for all logging writes to be sent to.
	bool SetLogFile(const std::string&);
private: // Private members
	EventLogger(void);
	
private: // Private static properties
	static EventLogger* glogger;
	
private: // Private properties
	Threading::ReadWriteMutex mutex;
	std::queue<std::string> logQueue;
	std::string logFile;
};
