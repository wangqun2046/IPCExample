#include "shmutils.h"

int main()
{
	using namespace boost::interprocess;

	shared_memory_object::remove(SHM_KEY);
	named_mutex::remove(MUTEX_KEY);
	named_condition::remove(CONDITION_KEY);

	return 0;
}
