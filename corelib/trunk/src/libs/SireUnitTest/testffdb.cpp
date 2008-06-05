
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include <QTextStream>

#include "SireError/exception.h"
#include "SireUnits/units.h"

#include "testffdb.h"

#include "SireMol/editmol.h"
#include "SireMol/molecule.h"
#include "SireMol/convertmolecule.h"

#include "SireDB/parameterdb.h"
#include "SireDB/parametertable.h"
#include "SireDB/using_parameters.hpp"
#include "SireDB/using_relationships.hpp"
#include "SireDB/assign_atoms.h"
#include "SireDB/assign_parameters.h"
#include "SireDB/matchhelpers.h"

#include "SireDB/relatemradb.h"
#include "SireDB/relateatomtypedb.h"

#include "SireDB/atomtypedb.h"
#include "SireDB/atomtypetable.h"

#include "SireMM/assign_bonds.h"
#include "SireMM/assign_angles.h"

#include "SireMM/chargedb.h"
#include "SireMM/chargetable.h"

#include "SireMM/ljdb.h"
#include "SireMM/ljtable.h"

#include "SireMM/bondtable.h"
#include "SireMM/bonddb.h"

#include "SireMM/angledb.h"
#include "SireMM/angletable.h"

#include "SireIO/pdb.h"
#include "SireIO/protoms.h"

using namespace SireTest;
using namespace SireMol;
using namespace SireMM;
using namespace SireDB;
using namespace SireCAS;
using namespace SireIO;
using namespace SireUnits;

TestFFDB::TestFFDB()
{}

TestFFDB::~TestFFDB()
{}

void TestFFDB::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestFFDB::runTests ) );
}

void TestFFDB::runTests()
{
    try
    {

    QTextStream strm(stdout);

    strm << QObject::tr("Running TestFFDB::runTests()\n");

    //load a TIP4P molecule
    QList<EditMol> editmols = PDB().read(":tip4p.pdb");
    
    BOOST_ASSERT( editmols.count() == 1 );
    BOOST_ASSERT( editmols[0].nAtoms() == 4 );
    BOOST_ASSERT( editmols[0].nResidues() == 1 );
    
    //automatically add bonds
    editmols[0].addAutoBonds();
    
    BOOST_ASSERT( editmols[0].nBonds() == 2 );
    
    //convert the Molecules
    MoleculeSet mols = flexibleAndMoleculeCutting(editmols);
    
    BOOST_ASSERT( mols.count() == 1 );
    
    Molecule tip4p = mols[0];
    
    BOOST_ASSERT( tip4p.nAtoms() == 4 );
    BOOST_ASSERT( tip4p.nResidues() == 1 );
    BOOST_ASSERT( tip4p.nBonds() == 2 );
    
    //load the parameter files
    ParameterDB db;
    
    ProtoMS().read( ":solvents.ff", db );
    
    BOOST_ASSERT( db.isA<ChargeDB>() );
    BOOST_ASSERT( db.isA<LJDB>() );
    BOOST_ASSERT( db.isA<AtomTypeDB>() );
    BOOST_ASSERT( db.isA<BondDB>() );
    BOOST_ASSERT( db.isA<AngleDB>() );
    BOOST_ASSERT( db.isA<RelateMRADB>() );
    BOOST_ASSERT( db.isA<RelateAtomTypeDB>() );
    
    ParameterTable params = db.assign( tip4p, 
                                       assign_parameters(
                                              assign_atoms( using_parameters<ChargeDB,LJDB,
                                                                             AtomTypeDB>(),
                                                            using_relationships<RelateMRADB>()
                                                          ),
                                              assign_bonds( using_parameters<BondDB>(),
                                                            using_relationships<RelateMRADB,
                                                                                RelateAtomTypeDB>()
                                                          ),
                                              assign_angles( using_parameters<AngleDB>(),
                                                             using_relationships<RelateMRADB,
                                                                                 RelateAtomTypeDB>()
                                                           )
                                                        ),
                                        mol_name == "T4P"
                                     );

    
    BOOST_ASSERT( params.isA<ChargeTable>() );
    BOOST_ASSERT( params.isA<LJTable>() );
    BOOST_ASSERT( params.isA<AtomTypeTable>() );
    BOOST_ASSERT( params.isA<BondTable>() );
    BOOST_ASSERT( params.isA<AngleTable>() );
    
    ChargeTable &chgs = params.asA<ChargeTable>();
    LJTable &ljs = params.asA<LJTable>();
    AtomTypeTable &typs = params.asA<AtomTypeTable>();
    BondTable &bonds = params.asA<BondTable>();
    AngleTable &angles = params.asA<AngleTable>();
    
    BOOST_ASSERT( not chgs.hasMissingParameters() );
    BOOST_ASSERT( not ljs.hasMissingParameters() );
    BOOST_ASSERT( not typs.hasMissingParameters() );
    BOOST_ASSERT( not bonds.hasMissingParameters() );
    BOOST_ASSERT( not angles.hasMissingParameters() );
    
    //print out the assignment log
    QString log = db.getLog();
    if (not log.isEmpty())
        strm << log << endl;

    //print out the parameters
    foreach( AtomIndex atom, params.molecule().atomVector() )
    {
        strm << QString("%1 %2 %3 %4\n")
                  .arg(atom.toString(), chgs[atom].toString(),
                       ljs[atom].toString(), typs[atom].toString());
    }
    
    foreach( Bond bond, params.molecule().bonds() )
    {
        strm << QString("%1 %2\n").arg(bond.toString(), bonds[bond].toString());
    }
    
    foreach( Angle angle, angles.angles() )
    {
        strm  << QString("%1 %2\n").arg(angle.toString(), angles[angle].toString());
    }
    
    //check that the parameters are correct
    BOOST_ASSERT( chgs[AtomIndex("O00",1)] == 0.0 );
    BOOST_ASSERT( chgs[AtomIndex("H01",1)] == 0.52 );
    BOOST_ASSERT( chgs[AtomIndex("H02",1)] == 0.52 );
    BOOST_ASSERT( chgs[AtomIndex("M03",1)] == -1.04 );

    BOOST_ASSERT( ljs[AtomIndex("O00",1)] == LJParameter(3.15363,0.155) );
    BOOST_ASSERT( ljs[AtomIndex("H01",1)] == LJParameter::dummy() );
    BOOST_ASSERT( ljs[AtomIndex("H02",1)] == LJParameter::dummy() );
    BOOST_ASSERT( ljs[AtomIndex("M03",1)] == LJParameter::dummy() );

    BOOST_ASSERT( typs[AtomIndex("O00",1)] == AtomType("OW","oxygen") );
    BOOST_ASSERT( typs[AtomIndex("H01",1)] == AtomType("HW","hydrogen") );
    BOOST_ASSERT( typs[AtomIndex("H02",1)] == AtomType("HW","hydrogen") );
    BOOST_ASSERT( typs[AtomIndex("M03",1)] == AtomType("??",0) );

    Symbol r = db.asA<BondDB>().r();
    
    BOOST_ASSERT( bonds[ Bond(AtomIndex("H01",1),AtomIndex("O00",1)) ] == 450*pow((r-0.9572),2) );
    BOOST_ASSERT( bonds[ Bond(AtomIndex("H02",1),AtomIndex("O00",1)) ] == 450*pow((r-0.9572),2) );
    
    Symbol theta = db.asA<AngleDB>().theta();
    
    BOOST_ASSERT( angles[ Angle(AtomIndex("H01",1),AtomIndex("O00",1),AtomIndex("H02",1)) ]
                            == 55*pow((theta - 104.52*degrees),2) );
    
    strm << "Done!\n";
    
    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
