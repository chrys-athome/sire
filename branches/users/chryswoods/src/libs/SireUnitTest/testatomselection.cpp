
#include <QDebug>

#include "testatomselection.h"

#include "SireMol/atomselection.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/resid.h"
#include "SireMol/resnum.h"
#include "SireMol/cgatomid.h"

#include "SireIO/pdb.h"

using namespace SireTest;
using namespace SireMol;
using namespace SireIO;

TestAtomSelection::TestAtomSelection()
{}

TestAtomSelection::~TestAtomSelection()
{}

void TestAtomSelection::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestAtomSelection::runTests ) );
}    

void TestAtomSelection::runTests()
{
    qDebug() << QObject::tr("Running AtomSelection tests...");
    
    QList<Molecule> mols = PDB().read(":p38.pdb");
    
    BOOST_CHECK_EQUAL( mols.count(), 1 );
    
    Molecule p38 = mols[0];
    
    AtomSelection s(p38);
    
    BOOST_CHECK( s.selectedAll() );
    BOOST_CHECK( not s.isEmpty() );
    
    BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() );
    
    uint nres = p38.nResidues();
    
    for (ResID i(0); i<nres; ++i)
    {
        ResNum resnum = p38.info().residueNumber(i);
        
        BOOST_CHECK_EQUAL( s.nSelected(resnum), p38.nAtoms(resnum) );
    }
    
    uint ncg = p38.nCutGroups();
    
    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( s.nSelected(i), p38.nAtoms(i) );
    }
    
    s.deselectAll();
    
    BOOST_CHECK( not s.selectedAll() );
    BOOST_CHECK( s.isEmpty() );
    
    for (ResID i(0); i<nres; ++i)
    {
        ResNum resnum = p38.info().residueNumber(i);
        
        BOOST_CHECK_EQUAL( s.nSelected(resnum), 0 );
    }
    
    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( s.nSelected(i), 0 );
    }
    
    qDebug() << QObject::tr("Finished AtomSelection tests");
}
