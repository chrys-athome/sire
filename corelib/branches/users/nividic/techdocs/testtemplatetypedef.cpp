
#include <iostream>

#include <boost/type_traits.hpp>

using namespace std;

namespace COMBINE
{

class null0
{
public:
	typedef null0 table;
	typedef null0 parameter_type;

	null0()
	{}
	
	~null0()
	{}

	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null1
{
public:
	typedef null1 table;
	typedef null1 parameter_type;

	null1()
	{}
	
	~null1()
	{}

	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null2
{
public:
	typedef null2 table;
	typedef null2 parameter_type;

	null2()
	{}
	
	~null2()
	{}

	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

}

template<class T0=COMBINE::null0, class T1=COMBINE::null1,
         class T2=COMBINE::null2>
class combine_parameter : public T0, public T1
{
public:
	combine_parameter() : T0(), T1()
	{}
	
	~combine_parameter()
	{}
};

template<class T0=COMBINE::null0, class T1=COMBINE::null1,
         class T2=COMBINE::null2>
class CombinedTable : public virtual T0::base_type
{
public:
	typedef typename T0::base_type base_type;
	typedef combine_parameter<T0,T1,T2> parameter_type;
	typedef typename T0::index_type index_type;

	typedef typename T0::parameter_type Parameter0;
	typedef typename T1::parameter_type Parameter1;
	typedef typename T2::parameter_type Parameter2;
	

	CombinedTable() : base_type()
	{}
	
	~CombinedTable()
	{}
	
	template<class T>
	T get(const index_type &index) const
	{
		if ( boost::is_same<T,Parameter0>::value )
			return Parameter0();
		else if ( boost::is_same<T,Parameter1>::value )
			return Parameter1();
		else
			return T();
	}
};

template<class T0=COMBINE::null0, class T1=COMBINE::null1,
         class T2=COMBINE::null2>
class combine : public CombinedTable<typename T0::table,
                                     typename T1::table,
									 typename T2::table>
{
public:
	typedef combine<T0,T1,T2> table;

	typedef T0 Database0;
	typedef T1 Database1;
	typedef T2 Database2;

	typedef typename T0::table Table0;
	typedef typename T1::table Table1;
	typedef typename T2::table Table2;

	typedef typename T0::parameter_type Parameter0;
	typedef typename T1::parameter_type Parameter1;
	typedef typename T2::parameter_type Parameter2;

	typedef combine_parameter<Parameter0,Parameter1,
	                          Parameter2> parameter_type;

	combine()
	{}
	
	~combine()
	{}
	
	void assign()
	{
		cout << "Assigning combined parameters...\n";
		cout << "All combined parameters have been assigned!\n";
	}
	
	void printInheritance() const
	{
		cout << "combine\n";
	}

	
};

class AtomTableBase
{
public:
	typedef AtomTableBase base_type;

	AtomTableBase()
	{}
	
	virtual ~AtomTableBase()
	{}
};

class BondTableBase
{
public:
	typedef BondTableBase base_type;

	BondTableBase()
	{}
	
	virtual ~BondTableBase()
	{}
};


class AtomType
{
public:
	AtomType()
	{}
	
	~AtomType()
	{}
};

class ChargeParameter
{
public:
	ChargeParameter()
	{}
	
	~ChargeParameter()
	{}
};

class LJParameter
{
public:
	LJParameter()
	{}
	
	~LJParameter()
	{}
};

class Expression
{
public:
	Expression()
	{}
	
	~Expression()
	{}
};

class AtomTypeTable : public virtual AtomTableBase
{
public:
	typedef int index_type;
	typedef AtomType parameter_type;

	AtomTypeTable() : AtomTableBase()
	{}
	
	~AtomTypeTable()
	{}
	
	void printInheritance() const
	{
		cout << "AtomTypeTable\n";
	}

	void assign()
	{
		cout << "Assigning atom types\n";
	}

	void assignAtomType()
	{
		this->assign();
	}
};

class ChargeTable : public virtual AtomTableBase
{
public:
	typedef int index_type;
	typedef ChargeParameter parameter_type;

	ChargeTable() : AtomTableBase()
	{}

	ChargeTable(const ChargeTable &other) : AtomTableBase(other)
	{
		cout << "ChargeTable copy constructor\n";
	}
	
	~ChargeTable()
	{}

	ChargeTable& operator=(const ChargeTable&)
	{
		cout << "Copying a ChargeTable\n";
		return *this;
	}
	
	void printInheritance() const
	{
		cout << "ChargeTable\n";
	}

	void assign()
	{
		cout << "Assigning charge parameters\n";
	}

	void assignCharge()
	{
		this->assign();
	}
};

class LJTable : public virtual AtomTableBase
{
public:
	typedef int index_type;
	typedef LJParameter parameter_type;

	LJTable() : AtomTableBase()
	{}
	
	~LJTable()
	{}
	
	void printInheritance() const
	{
		cout << "LJTable\n";
	}

	void assign()
	{
		cout << "Assigning LJ parameters\n";
	}

	void assignLJ()
	{
		this->assign();
	}
};

class BondTable : public virtual BondTableBase
{
public:
	typedef int index_type;
	typedef Expression parameter_type;

	BondTable() : BondTableBase()
	{}
	
	~BondTable()
	{}

	
	void printInheritance() const
	{
		cout << "BondTable\n";
	}

	void assign()
	{
		cout << "Assigning bond parameters\n";
	}

	void assignBond()
	{
		this->assign();
	}
};

class AtomTypeDB
{
public:
	typedef AtomTypeTable table;
	typedef AtomType parameter_type;
	
	AtomTypeDB()
	{}
	
	~AtomTypeDB()
	{}
};

class ChargeDB
{
public:
	typedef ChargeTable table;
	typedef ChargeParameter parameter_type;
	
	ChargeDB()
	{}
	
	~ChargeDB()
	{}
};

class LJDB
{
public:
	typedef LJTable table;
	typedef LJParameter parameter_type;
	
	LJDB()
	{}
	
	~LJDB()
	{}
};

class BondDB
{
public:
	typedef BondTable table;
	typedef Expression parameter_type;
	
	BondDB()
	{}
	
	~BondDB()
	{}
};

namespace ATOMS
{

class null0
{
public:
	typedef null0 table;
	typedef null0 parameter_type;

	null0()
	{}
	
	~null0()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null1
{
public:
	typedef null1 table;
	typedef null1 parameter_type;

	null1()
	{}
	
	~null1()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null2
{
public:
	typedef null2 table;
	typedef null2 parameter_type;

	null2()
	{}
	
	~null2()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

}

class atoms_base
{
public:
	atoms_base()
	{}
	
	virtual ~atoms_base()
	{}
	
	virtual void assignAtoms()=0;
};

template<class T0=ATOMS::null0, class T1=ATOMS::null1,
         class T2=ATOMS::null2>
class atoms : public T0::table, public T1::table, public T2::table
{
public:
	typedef typename T0::table Table0;
	typedef typename T1::table Table1;
	typedef typename T2::table Table2;

	typedef typename T0::parameter_type Parameter0;
	typedef typename T1::parameter_type Parameter1;
	typedef typename T2::parameter_type Parameter2;

	atoms() : Table0(), Table1(), Table2()
	{}
	
	~atoms()
	{}
	
	void printInheritance() const
	{
		cout << "atoms\n";
		Table0::printInheritance();
		Table1::printInheritance();
		Table2::printInheritance();
	}

	void assign()
	{
		cout << "Assigning atoms...\n";
		//get the ID numbers for the atoms being assigned
		
		//pass these to the tables to assign the parameters
		
		Table0::assign();
		Table1::assign();
		Table2::assign();
		cout << "All atoms have been assigned!\n";
	}

	void assignAtoms()
	{
		this->assign();
	}
};

namespace BONDS
{

class null0
{
public:
	typedef null0 table;
	typedef null0 parameter_type;

	null0()
	{}
	
	~null0()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null1
{
public:
	typedef null1 table;
	typedef null1 parameter_type;

	null1()
	{}
	
	~null1()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

class null2
{
public:
	typedef null2 table;
	typedef null2 parameter_type;

	null2()
	{}
	
	~null2()
	{}
	
	void printInheritance() const
	{}
protected:
	void assign()
	{}
};

}

template<class T0=BONDS::null0, class T1=BONDS::null1,
         class T2=BONDS::null2>
class bonds : public T0::table, public T1::table, public T2::table
{
public:
	typedef T0 Database0;
	typedef T1 Database1;
	typedef T2 Database2;

	typedef typename T0::table Table0;
	typedef typename T1::table Table1;
	typedef typename T2::table Table2;

	typedef typename T0::parameter_type Parameter0;
	typedef typename T1::parameter_type Parameter1;
	typedef typename T2::parameter_type Parameter2;

	bonds() : Table0(), Table1(), Table2()
	{}
	
	~bonds()
	{}
	
	void printInheritance() const
	{
		cout << "bonds\n";
		Table0::printInheritance();
		Table1::printInheritance();
		Table2::printInheritance();
	}

	void assign()
	{
		cout << "Assigning bonds...\n";
		Table0::assign();
		Table1::assign();
		Table2::assign();
		cout << "All bonds have been assigned!\n";
	}

	void assignBonds()
	{
		this->assign();
	}
};

namespace ASSIGN
{
class null0
{
public:
	null0()
	{}
	
	~null0()
	{}

	void printInheritance() const
	{}

protected:
	void assign()
	{}
};

class null1
{
public:
	null1()
	{}
	
	~null1()
	{}

	void printInheritance() const
	{}

protected:
	void assign()
	{}
};

class null2
{
public:
	null2()
	{}
	
	~null2()
	{}

	void printInheritance() const
	{}

protected:
	void assign()
	{}
};

}

template<class T0=ASSIGN::null0, class T1=ASSIGN::null1,
         class T2=ASSIGN::null2>
class assign_parameters : public virtual T0, public virtual T1, public T2
{
public:
	assign_parameters() : T0(), T1(), T2()
	{}
	
	~assign_parameters()
	{}
	
	void assign()
	{
		cout << "Assigning parameters...\n";
		T0::assign();
		T1::assign();
		T2::assign();
		cout << "All parameters have been assigned!\n";
	}
	
	void printInheritance() const
	{
		cout << "Inheritance\n";
		cout << "assign\n";
		T0::printInheritance();
		T1::printInheritance();
		T2::printInheritance();
	}
	
	template<class T>
	T extract() const
	{
		return static_cast<T>(*this);
	}
};

int main(void)
{

	assign_parameters< atoms<AtomTypeDB, combine<ChargeDB,LJDB> >,
	                   bonds<BondDB> > assigned;

	assigned.printInheritance();
	cout << endl;

	assigned.assign();
	cout << endl;
	
	assigned.assignAtoms();
	cout << endl;

	return 0;
}
