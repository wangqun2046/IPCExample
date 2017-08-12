#ifndef _SHMUTILS_H_
#define _SHMUTILS_H_

#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

const int SHM_SIZE = 65536;
const char* SHM_KEY = "SharedString";
const char* MUTEX_KEY = "mtx";
const char* CONDITION_KEY = "cnd";

struct shm_remove 
{
	~shm_remove() { boost::interprocess::shared_memory_object::remove("SharedString"); }
};

struct mutex_remove
{
	~mutex_remove() { boost::interprocess::named_mutex::remove("mtx"); }
};

struct condition_remove
{
	~condition_remove() { boost::interprocess::named_condition::remove("cnd"); }
};

#endif
