/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "testmatch.h"

#include "SireDB/matchmradata.h"
#include "SireDB/matchmra.h"
#include "SireDB/matchhelpers.h"

using namespace SireDB;
using namespace SireTest;

TestMatch::TestMatch() : TestBase()
{}

TestMatch::~TestMatch()
{}

void TestMatch::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestMatch::runTests ) );
}

void TestMatch::runTests()
{
    try
    {

    qDebug() << QObject::tr("Starting TestMatch::runTests()");
    
    MatchMRAData criteria = atom_name == "CA1" and res_name == "ALA";
    
    qDebug() << QObject::tr("Testing \"%1\"").arg(criteria.toString());
    
    BOOST_CHECK( not criteria.isEmpty() );
    BOOST_CHECK( criteria.hasResidueCriteria() );
    BOOST_CHECK( criteria.hasAtomCriteria() );
    BOOST_CHECK( not criteria.hasMoleculeCriteria() );
    BOOST_CHECK( criteria.nMatches() == 1 );
    
    MatchMRA match = criteria.getMatch(0);
    BOOST_CHECK( not match.isEmpty() );
    BOOST_CHECK( match.hasAtomCriteria() );
    BOOST_CHECK( match.hasResidueCriteria() );
    BOOST_CHECK( not match.hasMoleculeCriteria() );
    
    MatchMol molmatch = match.molecule();
    BOOST_CHECK( molmatch.isEmpty() );
    
    MatchRes resmatch = match.residue();
    BOOST_CHECK( not resmatch.isEmpty() );
    BOOST_CHECK( resmatch.hasNameMatch() );
    BOOST_CHECK( not resmatch.hasNumberMatch() );
    BOOST_CHECK( not resmatch.hasResResBonds() );
    BOOST_CHECK( not resmatch.hasNoResResBonds() );
    BOOST_CHECK( resmatch.name() == "ALA" );
    
    MatchAtom atommatch = match.atom();
    BOOST_CHECK( not atommatch.isEmpty() );
    BOOST_CHECK( atommatch.hasNameMatch() );
    BOOST_CHECK( not atommatch.hasNumberMatch() );
    BOOST_CHECK( atommatch.name() == "CA1" );
    
    criteria = mol_name == "p38" and 
               atom0_name == "CA" and atom1_name == "HA" and 
               res_name == "ASP";
    
    qDebug() << QObject::tr("Testing \"%1\"").arg(criteria.toString());
    
    BOOST_CHECK(not criteria.isEmpty());
    BOOST_CHECK(criteria.hasMoleculeCriteria());
    BOOST_CHECK(criteria.hasResidueCriteria());
    BOOST_CHECK(criteria.hasAtomCriteria());
    BOOST_CHECK(criteria.nMatches() == 2);
    
    match = criteria.getMatch(0);
    
    BOOST_CHECK(not match.isEmpty());
    BOOST_CHECK(match.hasMoleculeCriteria());
    BOOST_CHECK(match.hasResidueCriteria());
    BOOST_CHECK(match.hasAtomCriteria());
    
    molmatch = match.molecule();
    BOOST_CHECK(not molmatch.isEmpty());
    BOOST_CHECK(molmatch.name() == "p38");
    
    resmatch = match.residue();
    BOOST_CHECK(not resmatch.isEmpty());
    BOOST_CHECK(resmatch.hasNameMatch());
    BOOST_CHECK(not resmatch.hasNumberMatch());
    BOOST_CHECK(not resmatch.hasResResBonds());
    BOOST_CHECK(not resmatch.hasNoResResBonds());
    BOOST_CHECK(resmatch.name() == "ASP");
    
    atommatch = match.atom();
    BOOST_CHECK(not atommatch.isEmpty());
    BOOST_CHECK(atommatch.hasNameMatch());
    BOOST_CHECK(not atommatch.hasNumberMatch());
    BOOST_CHECK(atommatch.name() == "CA");
    
    match = criteria.getMatch(1);
    
    BOOST_CHECK(not match.isEmpty());
    BOOST_CHECK(match.hasMoleculeCriteria());
    BOOST_CHECK(match.hasResidueCriteria());
    BOOST_CHECK(match.hasAtomCriteria());
    
    molmatch = match.molecule();
    BOOST_CHECK(not molmatch.isEmpty());
    BOOST_CHECK(molmatch.name() == "p38");
    
    resmatch = match.residue();
    BOOST_CHECK(not resmatch.isEmpty());
    BOOST_CHECK(resmatch.hasNameMatch());
    BOOST_CHECK(not resmatch.hasNumberMatch());
    BOOST_CHECK(not resmatch.hasResResBonds());
    BOOST_CHECK(not resmatch.hasNoResResBonds());
    BOOST_CHECK(resmatch.name() == "ASP");
    
    atommatch = match.atom();
    BOOST_CHECK(not atommatch.isEmpty());
    BOOST_CHECK(atommatch.hasNameMatch());
    BOOST_CHECK(not atommatch.hasNumberMatch());
    BOOST_CHECK(atommatch.name() == "HA");
    
    qDebug() << QObject::tr("TestMatch::runTests() complete");
    
    }
    catch(const SireError::exception &error)
    {
        qDebug() << error.toString();
        throw;
    }
}
