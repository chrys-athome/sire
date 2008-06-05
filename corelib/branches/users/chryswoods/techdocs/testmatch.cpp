
#include <iostream>

using namespace std;

class Match
{
public:
	Match() : _mol(0), _res(0), _atm(0)
	{
		cout << "Match Constructor\n";
	}
	
	Match(const Match &other) 
	       : _mol(other._mol), _res(other._res), _atm(other._atm)
	{
		cout << "Copy constructor\n";
	}
	
	~Match()
	{}
	
	Match& operator=(const Match &other)
	{
		_mol = other._mol;
		_res = other._res;
		_atm = other._atm;
		
		cout << "Assignment operator\n";
		
		return *this;
	}
	
	Match& molecule(const char *mol)
	{
		_mol = mol;
		return *this;
	}
	
	Match& residue(const char *res)
	{
		_res = res;
		return *this;
	}
	
	Match& atom(const char *atm)
	{
		_atm = atm;
		return *this;
	}

	void printMe() const
	{
		cout << "Match Molecule(" << mol() << ") Residue (" 
		     << res() << ") Atom (" << atm() << ")\n";
	}
	
	int score() const
	{
		return ( (_mol != 0)<<2 ) | ( (_res != 0)<<1 ) | (_atm != 0);
	}
	
private:

	const char* mol() const
	{
		if (_mol)
			return _mol;
		else
			return "*";
	}

	const char* res() const
	{
		if (_res)
			return _res;
		else
			return "*";
	}

	const char* atm() const
	{
		if (_atm)
			return _atm;
		else
			return "*";
	}

	const char* _mol;
	const char* _res;
	const char* _atm;
};

void print(const Match &match)
{
	match.printMe();
	cout << match.score() << endl;
}

int main(void)
{
	print( Match().molecule("p38 kinase").residue("ARG") );

	print( Match().residue("ALA").atom("CA1") );

	print( Match().molecule("calix").residue("PHE").atom("CA1") );

/*  //Match can be extended to cope with bond, angle and dihedral

	print( Match2().molecule("p38 kinase").residue0("ALA").atom0("CA1")
	                                      .residue1("ALA").atom1("HA1").delta1(0) );

	print( Match3().residue0("ALA").atom0("CA1")
	               .residue1("ALA").atom1("N").delta1(0)
				   .residue2("ASP").atom2("C").delta1(1) );

	print( Match4().molecule("p38 kinase")
	               .residue0("ASP").atom0("CA1")
				   .residue1("ASP").atom1("C")   //delta1 is implicitly 0
				   .atom2("N").delta1(1)         //residue2 is *
				   .atom3("CA").delta2(1)        //residue3 is *


	//rank matches as they provide a 'score()' function, which gives a score
	//based on the strength of the match
*/

	return 0;
}
