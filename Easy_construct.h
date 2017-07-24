#ifndef EASY_CONSTRUCT_H
#define EASY_CONSTRUCT_H

#include <new>                      //for placement new
#include "Easy_type_traits.h"
#include "Easy_iterator.h"

template<class T1, class T2>
inline void construct(T1* p, T2& value)
{
	new(p)T1(value);
}

template<class T>
inline void destory(T* ptr)        //version one for destory
{
	ptr->~T();
}

template<class ForwardIterator>    //version two for destory
inline void destory(ForwardIterator first, ForwardIterator last)
{
	__destory(first, last, value_type(first)); //value_type() return pointer
}

template<class ForwardIterator, class T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destory_aux(first, last, trivial_destructor());
}

template<class ForwardIterator>
void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first<last; ++first)
		destory(&(*first));
}

template<class ForwardIterator>
inline void __destory_aux(ForwardIterator, ForwardIterator, __true_type)
{
#ifdef DEBUG
	std::cout << "trivial_destructor" << std::endl;
#endif
}

inline void destory(char*, char*){}
inline void destory(wchar_t*, wchar_t*){}



template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
	ForwardIterator cur = first;
	while (n--)
	{
		*cur = x;
		++cur;
	}
	return cur;

	//return fill_n(first, n, x);
}

template<class ForwardIterator, class Size, class T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
	//while(n--)
	//{
	//    construct(&(*first), x);
	//    ++first;
	//}
	//return first;

	ForwardIterator cur = first;
	for (; n>0; --n, ++cur)
		construct(&*cur, x);
	return cur;
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(first));
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	//return copy(first, last, result);

	ForwardIterator cur = result;
	while (first != last)
	{
		*cur = *first;
		++first;
		++cur;
	}
	return cur;
}

template<class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
	ForwardIterator cur = result;
	while (first != last)
	{
		construct(&*cur, *first);
		++first;
		++cur;
	}
	return cur;
}


/*

// memmove()
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
memmove(result, first, last-first);
return result+(last-first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
memmove(result, first, sizeof(wchar_t)*(last-first));
return result+(last-first);
}

*/


template<class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}

template<class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x, is_POD());
}

template<class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
	//fill(first, last, x);

	ForwardIterator cur = first;
	while (cur != last)
	{
		*cur = x;
		++cur;
	}
}

template<class ForwardIterator, class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
	ForwardIterator cur = first;
	while (cur != last)
	{
		construct(&*cur, x);
		++cur;
	}
}
#endif
