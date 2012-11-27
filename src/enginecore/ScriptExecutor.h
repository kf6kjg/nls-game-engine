/**
* \file
* \author Adam Martin
* \date 2012-04-14
* \brief ScriptExecutor declaration.
*/
#pragma once

// System Library Includes
#include <string>

// Application Library Includes

// Local Includes

// Forward Declarations
class asIScriptContext;
class asIScriptFunction;

// Typedefs

/**
* \brief A script context wrapper.
* \details Wraps a script context with several script building and executing functions
*/
class ScriptExecutor {
public:
	ScriptExecutor(void) { };
	~ScriptExecutor(void);

	/**
	* \brief Prepares a function to execute.
	*/
	int PrepareFunction(const std::string&, const std::string&);
	
	/**
	* \brief Prepares a function to execute.
	*/
	int PrepareFunction(asIScriptFunction*);

	/**
	* \brief Assign an argument for the function.
	*/
	int SetFunctionParam(int argIndex, void* param);
		
	/**
	* \brief Assign an argument for the function.
	*/
	int SetFunctionParam(int argIndex, float param);
		
	/**
	* \brief Assign an argument for the function.
	*/
	int SetFunctionParam(int argIndex, double param);

	/**
	* \brief Assign an argument for the function.
	*/
	int SetFunctionParam(int argIndex, unsigned int param);

	/**
	* \brief Executes the function.
	*/
	int ExecuteFunction();
	
	/**
	* \brief Gets the return value from the executed function.
	*/
	float GetReturnFloat();
	
	/**
	* \brief Sets the script context.
	*/
	void SetContext(asIScriptContext*);
private:
	asIScriptContext *ctx; /**< The script context */
};
