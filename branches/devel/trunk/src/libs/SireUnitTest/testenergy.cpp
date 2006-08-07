
#include <QDebug>

#include "testenergy.h"

#include "SireFF/energytypes.h"
#include "SireMaths/maths.h"
#include "SireUnits/units.h"
#include "SireUnits/convert.h"

using namespace SireTest;
using namespace SireUnits;
using namespace SireFF;
using namespace SireFF::EnergyType;

TestEnergy::TestEnergy()
{}

TestEnergy::~TestEnergy()
{}

void TestEnergy::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestEnergy::runTests ) );
}    

void TestEnergy::runTests()
{
    qDebug() << QObject::tr("Running Energy tests...");

    //should default initialise to 0.0,0.0,0.0
    Energy nrg;
     
    BOOST_CHECK_EQUAL(nrg.value(),0.0);
     
    nrg = Energy(1.0);
     
    BOOST_CHECK_EQUAL(nrg.value(),1.0);
     
    nrg = nrg + Energy(2.0);
     
    BOOST_CHECK_EQUAL(nrg.value(),3.0);
 
    nrg += 5.0;
     
    BOOST_CHECK_EQUAL(nrg.value(),8.0);

    nrg *= 2.0;
    BOOST_CHECK_EQUAL(nrg.value(),16.0);
 
    nrg /= 2;
    BOOST_CHECK_EQUAL(nrg.value(),8.0);
 
    Energy nrg2 = nrg * 2;
     
    BOOST_CHECK( nrg2 == (nrg / 2) * 4 );
 
    nrg = (nrg * 4) - nrg2;
    BOOST_CHECK( not (nrg != nrg2) );
     
    //now test the energy component classes
    CoulombEnergy cnrg;
    BOOST_CHECK_EQUAL( cnrg.value(), 0.0 );
     
    cnrg = CoulombEnergy(convertFrom(23.0, kJ_per_mol));
    BOOST_CHECK_EQUAL( convertTo(cnrg.value(), kJ_per_mol), 23.0 );
     
    cnrg = CoulombEnergy(1.0);
     
    cnrg += 46.5;
    BOOST_CHECK_EQUAL( cnrg.value(), 47.5 );
    
    cnrg -= 46.5;
    BOOST_CHECK_EQUAL( cnrg.value(), 1.0 );
     
    cnrg += CoulombEnergy(5.0);
    BOOST_CHECK_EQUAL( cnrg.value(), 6.0 );
     
    //test multiplication
    cnrg *= 4.0;
    BOOST_CHECK_EQUAL( cnrg.value(), 24.0 );
     
    cnrg *= CoulombEnergy(1.5);
    BOOST_CHECK_EQUAL( cnrg.value(), 36.0 );
     
    cnrg = CoulombEnergy(2.0);
    cnrg = SireMaths::pow_2(cnrg);
    BOOST_CHECK_EQUAL( cnrg.value(), 4.0 );
     
    EnergyPack<Coulomb,LJ> cljnrg( cnrg, 5.0 );
     
    BOOST_CHECK( cljnrg.get<Coulomb>() == cnrg );
    BOOST_CHECK( cljnrg.get<LJ>() == LJEnergy(5.0) );
    BOOST_CHECK( cljnrg == cljnrg.get<Coulomb>() + cljnrg.get<LJ>() );
     
    cljnrg = SireMaths::pow_2( cljnrg );
    BOOST_CHECK( cljnrg.get<Coulomb>() == SireMaths::pow_2(cnrg) );
    BOOST_CHECK( cljnrg.get<LJ>() == LJEnergy(25.0) );
    BOOST_CHECK( cljnrg == cljnrg.get<Coulomb>() + cljnrg.get<LJ>() );
     
    EnergyPack<Coulomb,LJ,Bond> cljbnrg;
    cljbnrg = cljnrg;
    
    BOOST_CHECK( cljnrg.get<Coulomb>() == cljbnrg.get<Coulomb>() );
    BOOST_CHECK( cljnrg.get<LJ>() == cljbnrg.get<LJ>() );
    BOOST_CHECK( cljbnrg.get<Bond>() == 0.0 );
    BOOST_CHECK( cljbnrg.get<Bond>() != 5.0 );
    BOOST_CHECK( cljbnrg == cljbnrg.get<Coulomb>() + cljbnrg.get<LJ>() + cljbnrg.get<Bond>() );
    
    cljnrg = EnergyPack<Coulomb,LJ>();
    
    BOOST_CHECK( cljnrg.get<Coulomb>() == 0.0 );
    BOOST_CHECK( cljnrg.get<LJ>() == 0.0 );
    BOOST_CHECK( cljnrg.value() == 0.0 );
    
    cljnrg += CoulombEnergy(2.0);
    
    BOOST_CHECK( SireMaths::pow_16(cljnrg) == SireMaths::pow_16(2.0) );
    BOOST_CHECK( SireMaths::pow_24(cljnrg) == SireMaths::pow_24(2.0) );
    
    cljnrg = CoulombEnergy(5.0);
    cljnrg = LJEnergy(3.0);
    
    EnergyPack<Coulomb,LJ> cljnrg2( 4.0, 2.0 );
    
    cljnrg = cljnrg * cljnrg2;
    
    BOOST_CHECK( cljnrg.get<Coulomb>() == 20.0 );
    BOOST_CHECK( cljnrg.get<LJ>() == 6.0 );
    BOOST_CHECK( cljnrg.get<Coulomb>() + cljnrg.get<LJ>() == cljnrg );

    //check that the sqrt and abs functions work correctly
    cnrg = 16.0;
    cnrg = std::sqrt(cnrg);
    
    BOOST_CHECK( cnrg == 4.0 );
    
    cnrg = -3.5;
    
    BOOST_CHECK( cnrg == -3.5 );
    BOOST_CHECK( std::abs(cnrg) == 3.5 );

    cljnrg = CoulombEnergy(16.0);
    cljnrg = LJEnergy(25.0);
    
    cljnrg = std::sqrt(cljnrg);
    BOOST_CHECK( cljnrg.get<Coulomb>() == 4.0 );
    BOOST_CHECK( cljnrg.get<LJ>() == 5.0 );
    
    cljnrg = CoulombEnergy(-4.9);
    cljnrg = LJEnergy(-23.6);
    
    BOOST_CHECK( cljnrg.get<Coulomb>() == -4.9 );
    BOOST_CHECK( cljnrg.get<LJ>() == -23.6 );
    
    cljnrg = std::abs(cljnrg);
    BOOST_CHECK( cljnrg.get<Coulomb>() == 4.9 );
    BOOST_CHECK( cljnrg.get<LJ>() == 23.6 );
    
    cljnrg = CoulombEnergy(16.0);
    cljnrg = LJEnergy(25.0);
    
    qDebug() << QObject::tr("Finished Energy tests");
}
