#include "Easy_functional.h"
#include "Easy_vector.h"

//#include <algorithm>
//using std::find_if;

#include <iostream>
using std::cout;
using std::endl;

template<class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
{
	while (first != last && !pred(*first)) ++first;
	return first;
}

int main()
{
	vector<int> ivec;
	for (int i(0); i != 10; ++i)
	{
		ivec.push_back(i);
	}
	for (const auto i : ivec)
		cout << i << " ";
	cout << endl;
	
	cout << *find_if(ivec.begin(), ivec.end(), bind2nd(greater<int>(), 3)) << endl;
	system("pause");
	return 0;
}