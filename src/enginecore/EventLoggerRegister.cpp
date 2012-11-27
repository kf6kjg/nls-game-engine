/**
* \file
* \author Ricky Curtice
* \date 2012-05-21
* \brief EventLogger registration function
*
*/

// System Library Includes

// Application Library Includes

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "ScriptEngine.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Helper function prototypes
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ScriptLog(const LOG_PRIORITY::TYPE&, const std::string&);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// The actual registration function
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \param[in] engine A pointer to the Angelscript engine instance.
*/
void EventLogger::RegisterScriptEngine(ScriptEngine* const engine) {
	asIScriptEngine* const as_engine = engine->GetasIScriptEngine();
	assert(as_engine != nullptr);
	
	int ret = 0;
	
	// Register Enum
	ret = as_engine->SetDefaultNamespace("Engine"); assert(ret >= 0);
	
	ret = as_engine->RegisterEnum("LOG_PRIORITY"); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "INFO",      ::LOG_PRIORITY::INFO); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "FLOW",      ::LOG_PRIORITY::FLOW); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "WARN",      ::LOG_PRIORITY::WARN); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "CONFIG",    ::LOG_PRIORITY::CONFIG); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "ERR",       ::LOG_PRIORITY::ERR); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "MISSRESS",  ::LOG_PRIORITY::MISSRESS); assert(ret >= 0);
	ret = as_engine->RegisterEnumValue("LOG_PRIORITY", "DEPRECATE", ::LOG_PRIORITY::DEPRECATE); assert(ret >= 0);
	
	// Register logger
	ret = as_engine->RegisterGlobalFunction("void LOG(const LOG_PRIORITY& in, const string& in)", asFUNCTION(ScriptLog), asCALL_CDECL); assert(ret >= 0);
	
	
	// Clean up after myself
	ret = as_engine->SetDefaultNamespace(""); assert(ret >= 0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Helper functions
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ScriptLog(const LOG_PRIORITY::TYPE& priority, const std::string& string) {
	asIScriptContext *ctx = asGetActiveContext();
	
	std::string message;
	
	if (ctx != nullptr) {
		asIScriptFunction* func = ctx->GetFunction();
		::EventLogger::GetEventLogger()->LogToDisk(priority, string, func->GetScriptSectionName(), static_cast<unsigned int>(ctx->GetLineNumber()), func->GetDeclaration());
	}
	else {
		::EventLogger::GetEventLogger()->LogToDisk(priority, string);
	}
}

