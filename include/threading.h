/**
 * \file
 * \author Ricky Curtice
 * \date 20120206
 * \brief Provides a simple set of interfaces for threading operations.
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <boost/thread.hpp>

namespace Threading {
	typedef boost::shared_mutex ReadWriteMutex;
	typedef boost::shared_lock<ReadWriteMutex> ReadLock;
	typedef boost::upgrade_lock<ReadWriteMutex> UpgradeLock;
	typedef boost::unique_lock<ReadWriteMutex> WriteLock;
	
	typedef boost::upgrade_to_unique_lock<ReadWriteMutex> UpgradeToUniqueLock;
	
	template<typename T>
	inline void Swap(T& left, T& right) {
		T temp = right;
		right = left;
		left = temp;
	}
}
