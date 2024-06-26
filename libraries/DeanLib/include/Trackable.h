#pragma once

#include <iostream>

/*
Dean Lawson
Champlain College
*/

class Trackable
{
public:
	Trackable() {}
	void* operator new(std::size_t size);
	void operator delete(void *ptr);
	void* operator new[](std::size_t size);
	void operator delete[](void *ptr);

	//for use with placement new
	void* operator new(std::size_t size, void* ptr){ return ptr; };
	void operator delete(void *ptr, void*ptr2){};
	void* operator new[](std::size_t size, void* ptr){ return ptr; };
	void operator delete[](void *ptr, void* ptr2){};

private:
};
