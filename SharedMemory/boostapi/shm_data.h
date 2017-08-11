#ifndef _SHM_DATA_H_
#define _SHM_DATA_H_

#include <boost/interprocess/managed_shared_memory.hpp>

enum exchange_type 
{
	SH, SZ, HK, NYSE, NASDAQ, AMEX
};

typedef boost::interprocess::allocator<char, 
	boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
typedef boost::interprocess::basic_string<char, 
	std::char_traits<char>, CharAllocator> ShmString;
struct stock_info 
{
	ShmString _name;
	ShmString _code;
	exchange_type _exchange_type;
	float _current_price;
	float _change_value;
	float _change_ratio;
};
typedef boost::interprocess::allocator<stock_info,
	boost::interprocess::managed_shared_memory::segment_manager> StockInfoAllocator;
typedef boost::interprocess::vector<stock_info, StockInfoAllocator> ShmStockInfoVector;

#endif
