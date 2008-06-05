
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "testcutgroup.h"

#include "SireMol/cutgroup.h"

using namespace SireTest;
using namespace SireMol;

TestCutGroup::TestCutGroup() : TestBase()
{}

TestCutGroup::~TestCutGroup()
{}

void TestCutGroup::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestCutGroup::runTests ) );
}

void TestCutGroup::runTests()
{
    qDebug() << QObject::tr("*** THERE ARE NO CUTGROUP TESTS ***");
    qDebug() << QObject::tr("***   NEW TESTS ARE NECESSARY   ***");
}
