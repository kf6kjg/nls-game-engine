/**
* \file
* \author Ricky Curtice
* \date 2012-02-26
* \brief A collection of functions that make registering the shared_ptr types much easier
*
*/
#pragma once

// Standard Includes
#include <cassert>

// Library Includes
#include <angelscript.h>
#include <angelscript/as_smart_ptr_wrapper.h>

// Local Includes

// Forward Declarations

// Typedefs

// Functions

// Helper functions

/**
* \brief Template placement new.
* \param[out] address The address where the template object will be created.
*/
template <typename T>
void construct(void* address) {
  new (address) T();
}

/**
* \brief Template dtor caller.
* \param[in] object Instance of template object to call dtor on.
*/
template <typename T>
void destroy(T* object) {
  object->~T();
}

/**
* \brief Template placement new with copy ctor.
* \param[out] address The address where the template object will be created.
* \param[in] other Instance of the template object to copy.
*/
template <typename T>
void copy_construct(void* address, T* other) {
  new (address) T(*other);
}

/**
* \brief Template move assignment.
* \param[out] lhs Where the template object will be assigned to.
* \param[in] rhs Where the template object is being assigned from.
*/
template <typename T>
void assign(T * lhs, T* rhs) {
  *lhs = *rhs;
}
