
#include <iostream>

using namespace std;

class A
{
public:
	A()
	{}
	
	~A()
	{}
private:
	int a;
};

class B
{
public:
	B()
	{}
	
	~B()
	{}
private:
	int b;
};

class C
{
public:
	C()
	{}
	
	~C()
	{}
};

class unspecified
{
public:
	unspecified()
	{}
	
	virtual ~unspecified()
	{}
};

template<class S, class T, class U = unspecified>
class combine : public S, public T, public U
{
public:
	combine() : S(), T(), U()
	{
		cout << "construct combine<S,T,U>()\n";
	}
		
	~combine()
	{}
};

template<>
class combine<A,B> : public A, public B
{
public:
	combine() : A(), B()
	{
		cout << "construct combine<A,B>()\n";
	}
	
	~combine()
	{}
	
	void specialFunction() const
	{
		cout << "Calling special function!\n";
	}
};

class manual : public A, public B
{
public:
	manual() : A(), B()
	{
		cout << "construct manual()\n";
	}
	
	~manual()
	{}
	
	void specialFunction() const
	{
		cout << "Calling manual::specialFunction!\n";
	}
};

template<class S, class T>
void callSpecial()
{
	combine<S,T> c;
	c.specialFunction();
}

template<class S, class T, class U>
void callSpecial()
{
	combine<S,T,U> c;
	c.specialFunction();
}

template<class S, class T, class U = unspecified>
class choose_holder_class
{
public:
	typedef combine<S,T,U> type;
};

template<>
class choose_holder_class<A,B>
{
public:
	typedef manual type;
};

template<>
class choose_holder_class<B,A>
{
public:
	typedef combine<A,B> type;
};

int main(void)
{
	combine<A,B> c;
	c.specialFunction();
	
	callSpecial<A,B>();

	combine<A,C> d;
	combine<B,A> e;
	
	combine<A,B,unspecified> f;
	callSpecial<A,B,unspecified>();
	
	//uncommenting the below two lines should
	//cause a compile error
	//callSpecial<A,C>();
	//callSpecial<B,A>();

	cout << "sizeof(combine<A,B>()) == " << sizeof( combine<A,B> ) << endl;
	cout << "sizeof(manual()) == " << sizeof( manual ) << endl;
	
	
	choose_holder_class<A,B>::type g;	
	choose_holder_class<B,A>::type h;	
	choose_holder_class<A,C>::type i;	
	
	return 0;
}
