/**
 * \file
 * \author Ricky Curtice
 * \brief Provides an implementation of nullptr from c++11 so that it can be used in older compilers.
 * 
 * This implementation of nullptr was copied from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr with the shared_ptr addition from a forum I've since lost track of...
 */

#include <tr1/memory>

const // It is a const object...
class nullptr_t {
public:
	template<class T>
	inline operator T*() const { // convertible to any type of null non-member pointer...
		return 0;
	}
	
	template<class C, class T>
	inline operator T C::*() const { // or any type of null member pointer...
		return 0;
	}
	
	template<typename T>
	operator std::tr1::shared_ptr<T>() const { // or shared_ptr
		return std::tr1::shared_ptr<T>();
	}
private:
	void operator&() const; // Can't take address of nullptr
} nullptr = {};
