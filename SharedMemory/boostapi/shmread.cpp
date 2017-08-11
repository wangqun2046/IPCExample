#include <iostream>
#include "shmutils.h"

int main()
{
	using namespace boost::interprocess;

	shm_remove remover;
	mutex_remove mutex_remover;
	condition_remove condition_remover;
	typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
	typedef basic_string<char, std::char_traits<char>, CharAllocator> string;
	bool running = true;
	named_mutex named_mtx(open_or_create, MUTEX_KEY);
	named_condition named_cnd(open_or_create, CONDITION_KEY);
	scoped_lock<named_mutex> lock(named_mtx);
	managed_shared_memory segment(open_or_create, SHM_KEY, SHM_SIZE);
	while (running)
	{
		std::cout << "Reader notify." << std::endl;
		named_cnd.notify_all();
		std::cout << "Reader wait ..." << std::endl;
		named_cnd.wait(lock);
		// read share memory	
		std::cout << "==========" << std::endl;
		std::pair<string *, std::size_t> p = segment.find<string>("Hello");
		if (p.first != NULL)
		{
			std::cout << "read string: [" << *p.first << "]" << std::endl;
			if (*p.first == "end") 
			{
				running = false;
			}
		}
	}
	named_cnd.notify_all();
	return 0;
}
