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
		// write share memory
		std::cout << "----------" << std::endl;
		string *s = segment.find_or_construct<string>("Hello")("", segment.get_segment_manager());
		s->clear();
		std::cout << "wait input: " << std::endl;
		std::cin >> *s;
		std::cout << "write string: [" << *s << "]" << std::endl;
		if (*s == "end")
		{
			running = false;
		}
		std::cout << "Writer notify." << std::endl;
		named_cnd.notify_all();
		std::cout << "Writer wait ..." << std::endl;
		named_cnd.wait(lock);
	}
	named_cnd.notify_all();
	return 0;
}
