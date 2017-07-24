#ifndef EASY_PAIR_H
#define EASY_PAIR_H

template<class T1, class T2>
struct pair
{
	pair(){}
	pair(T1 first, T2 second) :first(first), second(second){}
	T1 first;
	T2 second;
};

#endif
