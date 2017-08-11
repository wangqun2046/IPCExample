#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

int main(int argc, char** argv) 
{
	boost::interprocess::shared_memory_object shdmem(boost::interprocess::open_or_create,
		"shm_data", boost::interprocess::read_write);
	shdmem.truncate(1024);
	std::cout << "created shm: " << shdmem.get_name() << std::endl;
	boost::interprocess::offset_t size;
	if (shdmem.get_size(size)) 
	{
		std::cout << "shm size: " << size << std::endl;
	}
	boost::interprocess::mapped_region region(shdmem, boost::interprocess::read_write);
	char *s = static_cast<char*>(region.get_address());
	strcpy(s, "Hello, world!");
	std::cout << "write shm finished." << std::endl;
	boost::interprocess::mapped_region region2(shdmem, boost::interprocess::read_only);
	char *i2 = static_cast<char*>(region2.get_address());
	std::cout << "read shm finished, value: " << i2 << std::endl;
	boost::interprocess::shared_memory_object::remove("shm_data");
	std::cout << "removed shm: " << shdmem.get_name() << std::endl;
	return 0;
}
