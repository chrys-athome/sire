/**
  * @file
  *
  * C++ Implementation: TestVersion
  *
  * Description:
  * Implementation for TestVersion
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "testversion.h"

#include "SireBase/majminversion.h"

#include "SireError/errors.h"

#include <QTime>

using namespace SireTest;
using namespace SireBase;

TestVersion::TestVersion() : TestBase()
{}

TestVersion::~TestVersion()
{}

void TestVersion::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestVersion::runTests ) );
}

void TestVersion::runTests()
{
    try
    {

    qDebug() << "TestVersion::runTests() begin...";
    
    MajMinVersion ver;
    
    qDebug() << ver.toString();
    
    ver.incrementMinor();
    qDebug() << ver.toString();
    
    ver.incrementMinor();
    qDebug() << ver.toString();
    
    ver.incrementMinor();
    qDebug() << ver.toString();
    
    ver.incrementMajor();
    qDebug() << ver.toString();
    
    QTime t;
    
    t.start();
    for (int i=0; i<100000; ++i)
    {
        ver.incrementMinor();
    }
    
    int ms = t.elapsed();
    
    qDebug() << "100000 minors == " << ms;
    
    qDebug() << ver.toString();
    
    t.start();
    for (int i=0; i<100000; ++i)
    {
        ver.incrementMajor();
    }
    
    ms = t.elapsed();
    
    qDebug() << "100000 majors == " << ms;
    qDebug() << ver.toString();
    
    qDebug() << "TestVersion::runTests() complete";

    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
