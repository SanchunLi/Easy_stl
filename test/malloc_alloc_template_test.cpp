#define DEBUG

#include "Easy_simple_alloc.h"
#include "Easy_iterator.h"

#include <iostream>
#include <vector>

int main()
{

	std::vector<int, simple_alloc<int>> EZvec;
	for (int i(0); i != 10; ++i)
	{
		EZvec.push_back(i);
	}
	EZvec.resize(1000);
	EZvec.clear();
	system("pause");
	return 0;
}