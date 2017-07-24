#ifndef EASY_FUNCTIONAL_H
#define EASY_FUNCTIONAL_H

template<class Arg, class Result>
struct unary_function
{
	typedef Arg argument_type;
	typedef Result result_type;
};

template<class Arg1, class Arg2, class Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};


// ---------------------------Arithmetic-----------------------------------
// plus<T>
template<class T>
struct plus :public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x + y; }
};

// minus<T>
template<class T>
struct minus :public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x - y; }
};

// multiplies<T>
template<class T>
struct multiplies :public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x*y; }
};

// divides<T>
template<class T>
struct divides :public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x / y; }
};

// modulus<T>
template<class T>
struct modulus :public binary_function<T, T, T>
{
	T operator()(const T& x, const T&y) const { return x&y; }
};

// negate<T>
template<class T>
struct negate :public unary_function<T, T>
{
	T operator()(const T& x) const { return -x; }
};


// ---------------------------Relational-----------------------------------
//
template<class T>
struct equal_to :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x == y; }
};

template<class T>
struct not_equal_to :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x != y; }
};

template<class T>
struct greater :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x>y; }
};

template<class T>
struct greater_equal :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x >= y; }
};

template<class T>
struct less :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x<y; }
};

template<class T>
struct less_equal :public  binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x <= y; }
};


// ----------------------identity,select,project---------------------------
template<class T>
struct identity :public unary_function<T, T>
{
	const T& operator()(const T& x) const { return x; }
};

template<class Pair>
struct select1st :public unary_function<Pair, typename Pair::first_type>
{
	const typename Pair::first_type& operator()(const Pair& x) const
	{
		return x.first;
	}
};

template<class Pair>
struct select2nd :public unary_function<Pair, typename Pair::second_type>
{
	const typename Pair::second_type& operator()(const Pair& x) const
	{
		return x.second;
	}
};

template<class Arg1, class Arg2>
struct project1st :public binary_function<Arg1, Arg2, Arg1>
{
	Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

template<class Arg1, class Arg2>
struct project2nd :public binary_function<Arg1, Arg2, Arg2>
{
	Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};


//-------------------------function adapter---------------------
// unary_negate
template<class Predicate>
class unary_negate :public unary_function<typename Predicate::argument_type, bool>
{
protected:
	Predicate pred;
public:
	explicit unary_negate(const Predicate& x) :pred(x){}

	bool operator()(const typename Predicate::argument_type& x) const
	{
		return !pred(x);
	}
};

//aux function
template<class Predicate>
inline unary_negate<Predicate> not1(const Predicate& x)
{
	return unary_negate<Predicate>(x);
}


// binary_negate
template<class Predicate>
class binary_negate :public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool>
{
protected:
	Predicate pred;
public:
	explicit binary_negate(const Predicate& x) :pred(x){}

	bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const
	{
		return !pred(x, y);
	}

};

// aux function
template<class Predicate>
inline binary_negate<Predicate> not2(const Predicate& x)
{
	return binary_negate<Predicate>(x);
}


//(convert binary func to unary func)
// binder1st
template<class Operation>
class binder1st :public unary_function<typename Operation::second_argument_type, typename Operation::result_type>
{
protected:
	Operation op;
	typename Operation::first_argument_type value;
public:
	/*explicit*/
	binder1st(const Operation& op, const typename Operation::first_argument_type& x) :op(op), value(x){}

	typename Operation::result_type operator()(const typename Operation::second_argument_type& y) const
	{
		return op(value, y);
	}
};

//aux func
template<class Operation, class T>
inline binder1st<Operation> bind1st(const Operation& op, const T& x)
{
	typedef typename Operation::first_argument_type agr1_type;
	return binder1st<Operation>(op, arg1_type(x));
}


// binder2nd
template<class Operation>
class binder2nd :public unary_function<typename Operation::first_argument_type, typename Operation::result_type>
{
protected:
	Operation op;
	typename Operation::second_argument_type value;
public:
	binder2nd(const Operation& op, const typename Operation::second_argument_type& y) :op(op), value(y){}

	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const
	{
		return op(x, value);
	}
};

//aux func
template<class Operation, class T>
inline binder2nd<Operation> bind2nd(const Operation& op, const T& x)
{
	typedef typename Operation::second_argument_type arg2_type;
	return binder2nd<Operation>(op, arg2_type(x));
}


// compose1
template<class Operation1, class Operation2>
class unary_compose :public unary_function<
	typename Operation2::argument_type,
	typename Operation1::result_type>
{
protected:
	Operation1 op1;
	Operation2 op2;
public:
	unary_compose(const Operation1& op1, const Operation2& op2) :op1(op1), op2(op2){}

	typename Operation1::result_type operator()(const typename Operation2::argument_type& value) const
	{
		return op1(op2(value));
	}
};

// aux func
template<class Operation1, class Operation2>
inline unary_compose<Operation1, Operation2> compose1(const Operation1& op1, const Operation2& op2)
{
	return unary_compose<Operation1, Operation2>(op1, op2);
}


// compose2
template<class Operation1, class Operation2, class Operation3>
class binary_compose :public unary_function<
	typename Operation2::argument_type,
	typename Operation1::result_type>
{
protected:
	Operation1 op1;
	Operation2 op2;
	Operation3 op3;
public:
	binary_compose(const Operation1& op1, const Operation2& op2, const Operation3& op3) :op1(op1), op2(op2), op3(op3){}

	typename Operation1::result_type operator()(const typename Operation2::argument_type& value) const
	{
		return op1(op2(value), op3(value));
	}
};

//aux func
template<class Operation1, class Operation2, class Operation3>
inline binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3)
{
	return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}


// ptr_fun unary_function
template<class Arg, class Result>
class pointer_to_unary_function :public unary_function<Arg, Result>
{
protected:
	Result(*func)(Arg);  //pointer-to-func

public:
	pointer_to_unary_function(){}
	pointer_to_unary_function(Result(*f)(Arg)) :func(f){}

	Result operator()(Arg x) const
	{
		return (*func)(x);  // call the function
	}
};

template<class Arg, class Result>
pointer_to_unary_function<Arg, Result> prt_fun(Result(*f)(Arg))
{
	return pointer_to_unary_function<Arg, Result>(f);
}


//ptr_fun binary_function
template<class Arg1, class Arg2, class Result>
class pointer_to_binary_function :public binary_function<Arg1, Arg2, Result>
{
protected:
	Result(*ptr)(Arg1, Arg2);
public:
	pointer_to_binary_function(){}
	pointer_to_binary_function(Result(*f)(Arg1, Arg2)) :ptr(f){}

	Result operator()(Arg1 x1, Arg2 x2) const
	{
		return (*ptr)(x1, x2);
	}
};

template<class Arg1, class Arg2, class Result>
pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*f)(Arg1, Arg2))
{
	return pointer_to_binary_function<Arg1, Arg2, Result>(f);
}

#endif
