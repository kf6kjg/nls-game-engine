/**
* \file
* \author       Adam Martin
* \date         2012-04-14
* \brief		A script context wrapper.
* \details      Wraps a script context with several script building and executing functions
* 
*/


#include "ScriptExecutor.h"

// System Library Includes

// Application Library Includes
#include <angelscript.h>
#include <cassert>

// Local Includes

// Forward Declarations

// Typedefs

ScriptExecutor::~ScriptExecutor( void ) {
	if (this->ctx != nullptr) {
		if (this->ctx->Release() <= 0) {
			this->ctx = nullptr;
		}
	}
}

/**
* \param[in] funcdecl The function declaration to find and prepare.
* \param[in] modname The name of the module to search for the function.
* \return Negative number on failure.
*/
int ScriptExecutor::PrepareFunction( const std::string& funcdecl, const std::string& modname ) {
	// Find the function that is to be called.
	asIScriptModule *mod = this->ctx->GetEngine()->GetModule(modname.c_str());
	if (mod != nullptr) {
		asIScriptFunction* func = mod->GetFunctionByDecl(funcdecl.c_str());
		if (func != nullptr) {
			return this->ctx->Prepare(func);
		}
	}

	return -1;
}

/**
* \param[in] func The function to prepare.
* \return Negative number on failure.
*/
int ScriptExecutor::PrepareFunction( asIScriptFunction* func ) {
	// Find the function that is to be called. 
	if (func != nullptr) {
		return this->ctx->Prepare(func);
	}
	else {
		return -1;
	}
}

/**
* \param[in] argIndex The index of the argument.
* \param[in] param The argument.
* \return Negative number on failure.
*/
int ScriptExecutor::SetFunctionParam(int argIndex, void* param) {
	return this->ctx->SetArgObject(argIndex, param);
}

/**
* \param[in] argIndex The index of the argument.
* \param[in] param The argument.
* \return Negative number on failure.
*/
int ScriptExecutor::SetFunctionParam(int argIndex, float param) {
	return this->ctx->SetArgFloat(argIndex, param);
}

/**
* \param[in] argIndex The index of the argument.
* \param[in] param The argument.
* \return Negative number on failure.
*/
int ScriptExecutor::SetFunctionParam(int argIndex, double param) {
	return this->ctx->SetArgDouble(argIndex, param);
}

/**
* \param[in] argIndex The index of the argument.
* \param[in] param The argument.
* \return Negative number on failure.
*/
int ScriptExecutor::SetFunctionParam(int argIndex, unsigned int param) {
	return this->ctx->SetArgDWord(argIndex, param);
}

/**
* \return Negative number on failure.
*/
int ScriptExecutor::ExecuteFunction() {
	return this->ctx->Execute();
}

/**
* \return Value returned by the executed function.
*/
float ScriptExecutor::GetReturnFloat() {
	return this->ctx->GetReturnFloat();
}

/**
* \param[in] ctx The script context.
*/
void ScriptExecutor::SetContext( asIScriptContext* ctx ) {
	assert(ctx != nullptr);
	this->ctx = ctx;
}
