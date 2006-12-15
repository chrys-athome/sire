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
#include "SireBase/idmajminversion.h"
#include "SireBase/idversion.h"

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

    Incremint i0(0);

    MajMinVersion ver(&i0);

    BOOST_CHECK_EQUAL( ver.major(), 1 );
    BOOST_CHECK_EQUAL( ver.minor(), 0 );

    ver.incrementMinor();

    BOOST_CHECK_EQUAL( ver.major(), 1 );
    BOOST_CHECK_EQUAL( ver.minor(), 1 );

    ver.incrementMinor();

    BOOST_CHECK_EQUAL( ver.major(), 1 );
    BOOST_CHECK_EQUAL( ver.minor(), 2 );

    ver.incrementMinor();

    BOOST_CHECK_EQUAL( ver.major(), 1 );
    BOOST_CHECK_EQUAL( ver.minor(), 3 );

    ver.incrementMajor();

    BOOST_CHECK_EQUAL( ver.major(), 2 );
    BOOST_CHECK_EQUAL( ver.minor(), 0 );

    QTime t;

    t.start();
    for (int i=0; i<100000; ++i)
    {
        ver.incrementMinor();
    }

    int ms = t.elapsed();

    qDebug() << "100000 minors == " << ms;

    BOOST_CHECK_EQUAL( ver.major(), 2 );
    BOOST_CHECK_EQUAL( ver.minor(), 100000 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        ver.incrementMajor();
    }

    ms = t.elapsed();

    BOOST_CHECK_EQUAL( ver.major(), 100002 );
    BOOST_CHECK_EQUAL( ver.minor(), 0 );

    MajMinVersion ver2 = ver;

    BOOST_CHECK_EQUAL( ver.major(), 100002 );
    BOOST_CHECK_EQUAL( ver.minor(), 0 );

    ver2.incrementMinor();

    BOOST_CHECK_EQUAL( ver.major(), 100002 );
    BOOST_CHECK_EQUAL( ver.minor(), 0 );

    BOOST_CHECK_EQUAL( ver2.major(), 100002 );
    BOOST_CHECK_EQUAL( ver2.minor(), 1 );

    ver.incrementMinor();

    BOOST_CHECK_EQUAL( ver.major(), 100002 );
    BOOST_CHECK_EQUAL( ver.minor(), 2 );

    BOOST_CHECK_EQUAL( ver2.major(), 100002 );
    BOOST_CHECK_EQUAL( ver2.minor(), 1 );

    qDebug() << "100000 majors == " << ms;

    Incremint i1(0);
    IDVersion idver(&i1);

    BOOST_CHECK_EQUAL( idver.ID(), 1 );
    BOOST_CHECK_EQUAL( idver.version(), 0 );

    idver.incrementVersion();

    BOOST_CHECK_EQUAL( idver.ID(), 1 );
    BOOST_CHECK_EQUAL( idver.version(), 1 );

    idver.incrementVersion();

    BOOST_CHECK_EQUAL( idver.ID(), 1 );
    BOOST_CHECK_EQUAL( idver.version(), 2 );

    idver.incrementVersion();

    BOOST_CHECK_EQUAL( idver.ID(), 1 );
    BOOST_CHECK_EQUAL( idver.version(), 3 );

    idver.incrementID();

    BOOST_CHECK_EQUAL( idver.ID(), 2 );
    BOOST_CHECK_EQUAL( idver.version(), 0 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        idver.incrementVersion();
    }

    ms = t.elapsed();

    qDebug() << "100000 minors == " << ms;

    BOOST_CHECK_EQUAL( idver.ID(), 2 );
    BOOST_CHECK_EQUAL( idver.version(), 100000 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        idver.incrementID();
    }

    ms = t.elapsed();

    qDebug() << "100000 majors == " << ms;

    BOOST_CHECK_EQUAL( idver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver.version(), 0 );

    IDVersion idver2 = idver;

    BOOST_CHECK_EQUAL( idver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver.version(), 0 );

    idver2.incrementVersion();

    BOOST_CHECK_EQUAL( idver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver.version(), 0 );

    BOOST_CHECK_EQUAL( idver2.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver2.version(), 1 );

    idver.incrementVersion();

    BOOST_CHECK_EQUAL( idver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver.version(), 2 );

    BOOST_CHECK_EQUAL( idver2.ID(), 100002 );
    BOOST_CHECK_EQUAL( idver2.version(), 1 );

    Incremint i3(0);

    IDMajMinVersion idmajver(&i3);

    BOOST_CHECK_EQUAL( idmajver.ID(), 1 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    idmajver.incrementMinor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 1 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 1 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 1 );

    idmajver.incrementMinor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 1 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 2 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 2 );

    idmajver.incrementMajor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 1 );
    BOOST_CHECK_EQUAL( idmajver.major(), 2 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 2 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    idmajver.incrementMinor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 1 );
    BOOST_CHECK_EQUAL( idmajver.major(), 2 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 1 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 2 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 1 );

    idmajver.incrementID();

    BOOST_CHECK_EQUAL( idmajver.ID(), 2 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        idmajver.incrementMinor();
    }

    ms = t.elapsed();

    qDebug() << "100000 minors == " << ms;

    BOOST_CHECK_EQUAL( idmajver.ID(), 2 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 100000 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 100000 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        idmajver.incrementMajor();
    }

    ms = t.elapsed();

    qDebug() << "100000 majors == " << ms;

    BOOST_CHECK_EQUAL( idmajver.ID(), 2 );
    BOOST_CHECK_EQUAL( idmajver.major(), 100001 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 100001 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    t.start();
    for (int i=0; i<100000; ++i)
    {
        idmajver.incrementID();
    }

    ms = t.elapsed();

    qDebug() << "100000 IDs == " << ms;

    BOOST_CHECK_EQUAL( idmajver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    IDMajMinVersion idmajver2 = idmajver;

    BOOST_CHECK_EQUAL( idmajver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver2.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver2.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver2.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.version().minor(), 0 );

    idmajver2.incrementMinor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 0 );

    BOOST_CHECK_EQUAL( idmajver2.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver2.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.minor(), 1 );

    BOOST_CHECK_EQUAL( idmajver2.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.version().minor(), 1 );

    idmajver.incrementMinor();

    BOOST_CHECK_EQUAL( idmajver.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.minor(), 2 );

    BOOST_CHECK_EQUAL( idmajver.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver.version().minor(), 2 );

    BOOST_CHECK_EQUAL( idmajver2.ID(), 100002 );
    BOOST_CHECK_EQUAL( idmajver2.major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.minor(), 1 );

    BOOST_CHECK_EQUAL( idmajver2.version().major(), 1 );
    BOOST_CHECK_EQUAL( idmajver2.version().minor(), 1 );

    qDebug() << "TestVersion::runTests() complete";

    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
