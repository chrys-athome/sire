
#include <iostream>

using namespace std;

class Parent
{
public:
	Parent()
	{}
	
	virtual ~Parent()
	{}
};

class A
{
public:
	template<class T>
	bool isA() const
	{
		return dynamic_cast<const T*>(this) != 0;
	}

	A()
	{}
		
	virtual ~A()
	{}
	
	void check()
	{
		if (not isA<Parent>())
			cout << "NO PARENT!\n";
		else
			cout << "PARENT! :-)\n";
	}
	
};

class B : private virtual A
{
public:
	B() : A()
	{}
	
	~B()
	{}
	
	void check()
	{
		cout << "Check B: ";
		A::check();
	}
};

class C : private virtual A
{
public:
	C() : A()
	{}
	
	~C()
	{}
	
	void check()
	{
		cout << "Check C: ";
		A::check();
	}
};

class D : private virtual A
{
public:
	D() : A()
	{}
	
	~D()
	{}
	
	void check()
	{
		cout << "Check D: ";
		A::check();
	}
};

class E : public virtual A  //change this to 'protected' or
{                           //'private' inheritance to prevent
public: 					//Test2 from finding its parent
	E() : A()
	{}
	
	~E()
	{}
	
	void check()
	{
		cout << "Check E: ";
		A::check();
	}
};


/////////////
/////////////

template<class R, class S>
class Part : public R, public S
{
public:
	Part() : R(), S()
	{}
	
	~Part()
	{}
	
	void check()
	{
		R::check();
		S::check();
	}
};

class Test1 : public Parent, public B, public C
{
public:
	Test1() : Parent(), B(), C()
	{
		check();
	}
	
	~Test1()
	{}

	void check()
	{
		cout << "Test1::check()\n";
		B::check();
		C::check();
	}
};

class Test2 : public Parent, public Part<B,C>, public Part<D,E>
{
public:
	Test2() : Parent(), Part<B,C>(), Part<D,E>()
	{
		check();
	}
	
	~Test2()
	{}
	
	void check()
	{
		cout << "Test2::check()\n";
		Part<B,C>::check();
		Part<D,E>::check();
	}
};

class Test3 : public Parent, public B, public E
{
public:
	Test3() : Parent(), B(), E()
	{
		check();
	}
	
	~Test3()
	{}
	
	void check()
	{
		cout << "Test3::check()\n";
		B::check();
		E::check();
	}
};

int main(void)
{
	//Test1 won't find its parent as all of the 
	// class A's that are inherited are private!
	Test1 test1;

	//Test2 will find the parent as one of the 
	//four class A's that are inherited are public!
	Test2 test2;

	//Test3 will find the parent as one of the
	//two class A's that are inherited are public!
	Test3 test3;

	return 0;
}
