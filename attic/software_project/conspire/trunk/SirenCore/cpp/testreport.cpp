/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/testreport.h"
#include "Siren/mutable.hpp"
#include "Siren/editor.hpp"
#include "Siren/siren.hpp"
#include "Siren/class.h"
#include "Siren/number.h"
#include "Siren/exception.h"
#include "Siren/obj.hpp"
#include "Siren/object.hpp"

#include "third_party/fcmp.h"

using namespace Siren;
using namespace Siren::Qt;

namespace Siren
{
    namespace detail
    {
    
        /** This internal class is used to hold the implicitly shared data
            of a TestReport */
        class TestReportData : public imp_shared_data
        {
        public:
            TestReportData() : imp_shared_data()
            {}
            
            TestReportData(const Class &c, 
                           const List<Obj>::Type &reports)
                  : imp_shared_data(),
                    clas(c), items(reports)
            {}
                           
            TestReportData(const TestReportData &other)
                  : imp_shared_data(), 
                    clas(other.clas)
            {}
            
            ~TestReportData()
            {}
            
            TestReport successes() const
            {
                return TestReport();
            }
            
            TestReport failures() const
            {
                return TestReport();
            }

            Obj operator[](int i) const
            {
                return items.at( wrap_index(i, items.count(), CODELOC) );
            }
            
            Obj at(int i) const
            {
                return items.at( wrap_index(i, items.count(), CODELOC) );
            }
            
            int count() const
            {
                return items.count();
            }
            
            int size() const
            {
                return items.count();
            }
            
            bool passed() const
            {
                for (List<Obj>::const_iterator it = items.constBegin();
                     it != items.constEnd();
                     ++it)
                {
                    if (it->asA<TestReportItem>().failed())
                        return false;
                }
                
                return true;
            }
            
            bool failed() const
            {
                for (List<Obj>::const_iterator it = items.constBegin();
                     it != items.constEnd();
                     ++it)
                {
                    if (it->asA<TestReportItem>().failed())
                        return true;
                }
                
                return false;
            }
            
            bool isEmpty() const
            {
                return items.isEmpty();
            }
            
            int numberOfTests() const
            {
                return items.count();
            }
            
            int numberOfSuccesses() const
            {
                int n = 0;
                
                for (List<Obj>::const_iterator it = items.constBegin();
                     it != items.constEnd();
                     ++it)
                {
                    if (it->asA<TestReportItem>().passed())
                        ++n;
                }
                
                return n;
            }
            
            int numberOfFailures() const
            {
                int n = 0;
                
                for (List<Obj>::const_iterator it = items.constBegin();
                     it != items.constEnd();
                     ++it)
                {
                    if (it->asA<TestReportItem>().failed())
                        ++n;
                }
                
                return n;
            }
            
            /** The class being tested */
            Class clas;

            /** The list of test report items */
            List<Obj>::Type items;
            
        private:
            TestReportData& operator=(const TestReportData &other)
            {
                return *this;
            }
            
        }; // end of TestReportData
    
    } // end of namespace detail

} // end of namespace Siren

///////////
/////////// Implementation of Siren::TestReportItem
///////////

REGISTER_SIREN_VIRTUAL_CLASS( Siren::TestReportItem )

/** Null constructor */
TestReportItem::TestReportItem() : Object()
{}

/** Construct an item with the passed description */
TestReportItem::TestReportItem(const String &description)
               : Object(), desc(description)
{}

/** Copy constructor */
TestReportItem::TestReportItem(const TestReportItem &other)
               : Object(other), desc(other.desc)
{}

/** Destructor */
TestReportItem::~TestReportItem()
{}

/** Return the description of the test */
const String& TestReportItem::description() const
{
    return desc;
}

/** Return whether or not the test failed */
bool TestReportItem::failed() const
{
    return not this->passed();
}

/** Copy assignment operator */
void TestReportItem::copy_object(const TestReportItem &other)
{
    desc = other.desc;
    super::copy_object(other);
}

/** Comparison operator */
bool TestReportItem::compare_object(const TestReportItem &other) const
{
    return desc == other.desc and super::compare_object(other);
}

///////////
/////////// Implementation of Siren::TestPassedItem
///////////

REGISTER_SIREN_CLASS( Siren::TestPassedItem )

/** Null constructor */
TestPassedItem::TestPassedItem() : TestReportItem()
{}

/** Construct a report of a passed test with the passed description */
TestPassedItem::TestPassedItem(const String &description)
               : TestReportItem(description)
{}

/** Copy constructor */
TestPassedItem::TestPassedItem(const TestPassedItem &other)
               : TestReportItem(other)
{}

/** Destructor */
TestPassedItem::~TestPassedItem()
{}

/** Return whether or not the test passed */
bool TestPassedItem::passed() const
{
    return true;
}

/** Return a string representation of this item */
String TestPassedItem::toString() const
{
    return Siren::tr("%1: (passed)").arg(this->description());
}

/** Copy assignment operator */
void TestPassedItem::copy_object(const TestPassedItem &other)
{
    super::copy_object(other);
}

/** Comparison operator */
bool TestPassedItem::compare_object(const TestPassedItem &other) const
{
    return super::compare_object(other);
}

///////////
/////////// Implementation of Siren::TestFailedItem
///////////

REGISTER_SIREN_CLASS( Siren::TestFailedItem )

/** Null constructor */
TestFailedItem::TestFailedItem() : TestReportItem()
{}

/** Construct a report of a failed test with the passed description */
TestFailedItem::TestFailedItem(const String &description)
               : TestReportItem(description)
{}

/** Copy constructor */
TestFailedItem::TestFailedItem(const TestFailedItem &other)
               : TestReportItem(other)
{}

/** Destructor */
TestFailedItem::~TestFailedItem()
{}

/** Return whether or not the test passed */
bool TestFailedItem::passed() const
{
    return false;
}

/** Return a string representation of the test */
String TestFailedItem::toString() const
{
    return Siren::tr("%1: (failed!)").arg(this->description());
}

/** Copy assignment operator */
void TestFailedItem::copy_object(const TestFailedItem &other)
{
    super::copy_object(other);
}

/** Comparison operator */
bool TestFailedItem::compare_object(const TestFailedItem &other) const
{
    return super::compare_object(other);
}

///////////
/////////// Implementation of Siren::TestFailedCompareItem
///////////

REGISTER_SIREN_CLASS( Siren::TestFailedCompareItem )

/** Null constructor */
TestFailedCompareItem::TestFailedCompareItem() : TestReportItem(), tol(0)
{}

/** Construct to report failed comparison of two logical values */
TestFailedCompareItem::TestFailedCompareItem(bool result, bool expected,
                                             const String &description)
                      : TestReportItem(description),
                        expected_val( Number(expected) ),
                        obtained_val( Number(result) ),
                        tol(0)
{}
  
/** Construct to report failed comparison of two integer values */
TestFailedCompareItem::TestFailedCompareItem(int result, int expected,
                                             const String &description)
                      : TestReportItem(description),
                        expected_val( Number(expected) ),
                        obtained_val( Number(result) ),
                        tol(0)
{}
  
/** Construct to report failed comparison of two floating point values */
TestFailedCompareItem::TestFailedCompareItem(double result, double expected,
                                             double tolerance, const String &description)
                      : TestReportItem(description),
                        expected_val( Number(expected) ),
                        obtained_val( Number(result) ),
                        tol(tolerance)
{}
  
/** Construct to report failed comparison of two Siren objects */
TestFailedCompareItem::TestFailedCompareItem(const Object &result, const Object &expected,
                                             const String &description)
                      : TestReportItem(description),
                        expected_val(expected),
                        obtained_val(result),
                        tol(0)
{}

/** Copy constructor */
TestFailedCompareItem::TestFailedCompareItem(const TestFailedCompareItem &other)
                      : TestReportItem(other),
                        expected_val(other.expected_val),
                        obtained_val(other.obtained_val),
                        tol(other.tol)
{}

/** Destructor */
TestFailedCompareItem::~TestFailedCompareItem()
{}

/** Return whether or not the test passed */
bool TestFailedCompareItem::passed() const
{
    return false;
}

/** Return whether or not this was comparison of two logical types */
bool TestFailedCompareItem::isBooleanComparison() const
{
    if (expected_val.isA<Number>() and obtained_val.isA<Number>())
    {
        return expected_val.asA<Number>().isBool() and
               obtained_val.asA<Number>().isBool();
    }
    else
        return false;
}

/** Return whether or not this was a comparison of two number types */
bool TestFailedCompareItem::isNumberComparison() const
{
    if (expected_val.isA<Number>() and obtained_val.isA<Number>())
    {
        return (expected_val.asA<Number>().isFloat() or  
                expected_val.asA<Number>().isInteger()) and
               (obtained_val.asA<Number>().isFloat() or
                obtained_val.asA<Number>().isInteger());
    }
    else
        return false;
}

/** Return whether or not this was a comparison of two Siren objects */
bool TestFailedCompareItem::isObjectComparison() const
{
    return not (expected_val.isA<Number>() and obtained_val.isA<Number>());
}

/** Return the expected value for the test */
const Object& TestFailedCompareItem::expected() const
{
    return expected_val;
}

/** Return the actual result used in the test */
const Object& TestFailedCompareItem::result() const
{
    return obtained_val;
}

/** Return the tolerance used if this was a comparison of floating point numbers */
double TestFailedCompareItem::tolerance() const
{
    return tol;
}

/** Return a description of this test */
String TestFailedCompareItem::toString() const
{
    if (expected_val.isA<Number>() and obtained_val.isA<Number>())
    {
        return Siren::tr("%1: (failed! %2 != %3)")
                    .arg(this->description(),
                         obtained_val.toString(),
                         expected_val.toString());
    }
    else if (expected_val.what() != obtained_val.what())
    {
        return Siren::tr("%1: (failed! Objects of type %2 and %3 are not equal)")
                    .arg(this->description(),
                         obtained_val.what(),
                         expected_val.what());
    }
    else
    {
        return Siren::tr("%1: (failed! Two objects of type %2 are not equal)")
                    .arg(this->description(),
                         obtained_val.what());
    }
}

/** Copy assignment operator */
void TestFailedCompareItem::copy_object(const TestFailedCompareItem &other)
{
    expected_val = other.expected_val;
    obtained_val = other.obtained_val;
    tol = other.tol;
    super::copy_object(other);
}

/** Comparison operator */
bool TestFailedCompareItem::compare_object(const TestFailedCompareItem &other) const
{
    return expected_val == other.expected_val and
           obtained_val == other.obtained_val and
           tol == other.tol and
           super::compare_object(other);
}

///////////
/////////// Implementation of Siren::TestFailedExceptionItem
///////////

REGISTER_SIREN_CLASS( Siren::TestFailedExceptionItem )

/** Null constructor */
TestFailedExceptionItem::TestFailedExceptionItem() : TestReportItem()
{}

/** Construct a report of a failure caused by the capture of the
    passed exception */
TestFailedExceptionItem::TestFailedExceptionItem(const Exception &e)
                        : TestReportItem( Siren::tr("Caught unexpected exception!") ),
                          excptn(e)
{}

/** Construct a report of a failure caused by the capture of the passed
    exception, with the passed test description */
TestFailedExceptionItem::TestFailedExceptionItem(const Exception &e,
                                                 const String &description)
                        : TestReportItem(description), excptn(e)
{}

/** Copy constructor */
TestFailedExceptionItem::TestFailedExceptionItem(const TestFailedExceptionItem &other)
                        : TestReportItem(other), excptn(other.excptn)
{}

/** Destructor */
TestFailedExceptionItem::~TestFailedExceptionItem()
{}

/** Return whether or not the test passed */
bool TestFailedExceptionItem::passed() const
{
    return false;
}

/** Return the exception that was caught during the test */
const Exception& TestFailedExceptionItem::exception() const
{
    return excptn.asA<Exception>();
}

/** Return a string representation of the exception */
String TestFailedExceptionItem::toString() const
{
    if (excptn.isNone())
    {
        return Siren::tr("%1: (failed!)").arg(this->description());
    }
    else
    {
        return Siren::tr("%1: (failed! Exception %1 was thrown)")
                        .arg(this->description(), 
                             excptn.asA<Exception>().what());
    }
}

/** Copy assignment operator */
void TestFailedExceptionItem::copy_object(const TestFailedExceptionItem &other)
{
    excptn = other.excptn;
    super::copy_object(other);
}

/** Comparison operator */
bool TestFailedExceptionItem::compare_object(const TestFailedExceptionItem &other) const
{
    return excptn == other.excptn and super::compare_object(other);
}

///////////
/////////// Implementation of Siren::TestReport
///////////

REGISTER_SIREN_CLASS( Siren::TestReport )

/** Constructor */
TestReport::TestReport()
           : TestReportItem(), 
             Interfaces< Mutable<TestReportEditor,TestReport> >(),
             d( new detail::TestReportData() )
{}

/** Construct to report, not on a class, but on a group of tests */
TestReport::TestReport(const String &description)
           : TestReportItem(description),
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{
    d = new detail::TestReportData();
}

/** Construct to report on the tests of the passed class */
TestReport::TestReport(const Class &c)
           : TestReportItem( Siren::tr("Tests for class \"%1\"").arg(c.name()) ),
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{
    d = new detail::TestReportData();
    d->clas = c;
}

/** Construct to report on the tests of the passed class */
TestReport::TestReport(const Class &c, const String &description)
           : TestReportItem(description),
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{
    d = new detail::TestReportData();
    d->clas = c;
}

/** Construct from the passed editor */
TestReport::TestReport(TestReportEditor &editor)
           : TestReportItem(),
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{
    this->operator=( editor.commit() );
}

/** Copy constructor */
TestReport::TestReport(const TestReport &other)
           : TestReportItem(other),
             Interfaces< Mutable<TestReportEditor,TestReport> >(other),
             d(other.d)
{}

/** Destructor */
TestReport::~TestReport()
{}

/** Return the ith test item in this report

    \throw Siren::invalid_index
*/
Obj TestReport::operator[](int i) const
{
    return d->operator[](i);
}

/** Return the ith test item in this report

    \throw Siren::invalid_index
*/
Obj TestReport::at(int i) const
{
    return d->at(i);
}

/** Return the number of test report items in this report */
int TestReport::count() const
{
    return d->count();
}

/** Return the number of test report items in this report */
int TestReport::size() const
{
    return d->size();
}

/** Return a string representation of the report */
String TestReport::toString() const
{
    if (this->isEmpty())
    {
        return Siren::tr("%1: (passed, but no tests were performed!)")
                            .arg(this->description());
    }
    else if (this->passed())
    {
        return Siren::tr("%1: (passed)").arg(this->description());
    }
    else
    {
        return Siren::tr("%1: (failed! numberOfFailures() == %2)")
                            .arg(this->description()).arg(this->numberOfFailures());
    }
}

/** Copy assignment operator */
void TestReport::copy_object(const TestReport &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool TestReport::compare_object(const TestReport &other) const
{
    return d == other.d and super::compare_object(other);
}

/** Return whether or not all of the tests passed */
bool TestReport::passed() const
{
    return d->passed();
}

/** Return whether or not any of the tests failed */
bool TestReport::failed() const
{
    return d->failed();
}
    
/** Return whether or not this report is empty */
bool TestReport::isEmpty() const
{
    return d->isEmpty();
}

/** Return the total number of test report items in this report */
int TestReport::numberOfTests() const
{
    return d->numberOfTests();
}

/** Return the number of passed tests in this report */
int TestReport::numberOfSuccesses() const
{
    return d->numberOfSuccesses();
}

/** Return the number of failed tests in this report */
int TestReport::numberOfFailures() const
{
    return d->numberOfFailures();
}

/** Return the report of all of the sucessesful tests */
TestReport TestReport::successes() const
{
    return d->successes();
}

/** Return the report of all of the failed tests */
TestReport TestReport::failures() const
{
    return d->failures();
}

/** Return the class that was tested */
Class TestReport::testedClass() const
{
    return d->clas;
}

/** Perform the tests of the TestReport class */
void TestReport::test(TestReportEditor &report) const
{
    TestReport test;
    
    report.testTrue( test.passed(), Siren::tr("Empty report has no failures") );
    report.testFalse( test.failed(), Siren::tr("Empty report has not failed") );
    
    test = test.edit().addFailure( Siren::tr("Test failure") );
    
    report.testFalse( test.passed(), 
                      Siren::tr("Report with one failure has not passed") );
    report.testTrue( test.failed(), Siren::tr("Report with one failure has failed") );
    
    report.testEqual( test.numberOfTests(), 1, Siren::tr("Number of tests is 1") );
    report.testEqual( test.numberOfSuccesses(), 0, Siren::tr("Number of passes is 0") );
    report.testEqual( test.numberOfFailures(), 1, Siren::tr("Number of failures is 1") );

    
}

/** Perform the stress tests of the TestReport class */
void TestReport::stressTest(TestReportEditor &report) const
{
    TestReport::test(report);
}

///////////
/////////// Implementation of Siren::TestReportEditor
///////////

/** Constructor */
TestReportEditor::TestReportEditor()
                 : Editor<TestReportEditor,TestReport>()
{
    d = new detail::TestReportData();
}

/** Construct from the passed TestReport - this extracts the data
    from TestReport and packages it as an editable block */
TestReportEditor::TestReportEditor(const TestReport &report)
                 : Editor<TestReportEditor,TestReport>()
{
    d = report.d;
    desc = report.description();
}

/** Copy constructor */
TestReportEditor::TestReportEditor(const TestReportEditor &other)
                 : Editor<TestReportEditor,TestReport>(other),
                   d(other.d), desc(other.desc)
{}

/** Destructor */
TestReportEditor::~TestReportEditor()
{}

/** Commit the changes - this coverts the editable data back to read-only
    data and updates this editor that state. It then returns the read-only
    data as a TestReport object */
TestReport TestReportEditor::commit()
{
    TestReport report(desc);
    report.d = d;
    this->updateOriginal(report);
    
    return report;
}

/** Copy assignment operator */
TestReportEditor& TestReportEditor::operator=(const TestReportEditor &other)
{
    d = other.d;
    desc = other.desc;
    return *this;
}

/** Comparison operator */
bool TestReportEditor::operator==(const TestReportEditor &other) const
{
    return d == other.d and desc == other.desc;
}

/** Return the ith item in the report

    \throw Siren::invalid_index
*/
Obj TestReportEditor::operator[](int i) const
{
    return d->operator[](i);
}

/** Return the ith item in the report

    \throw Siren::invalid_index
*/
Obj TestReportEditor::at(int i) const
{
    return d->at(i);
}

/** Return the number of items in the report */
int TestReportEditor::count() const
{
    return d->count();
}

/** Return the number of items in the report */
int TestReportEditor::size() const
{
    return d->size();
}

/** Return a description of the tests */
const String& TestReportEditor::description() const
{
    return desc;
}

/** Return a string representation of the report */
String TestReportEditor::toString() const
{
    if (this->isEmpty())
    {
        return Siren::tr("%1: (passed, but no tests were performed!) [EDIT]")
                            .arg(this->description());
    }
    else if (this->passed())
    {
        return Siren::tr("%1: (passed) [EDIT]").arg(this->description());
    }
    else
    {
        return Siren::tr("%1: (failed! numberOfFailures() == %2) [EDIT]")
                            .arg(this->description()).arg(this->numberOfFailures());
    }
}

/** Return whether or not all of the tests so-far have passed */
bool TestReportEditor::passed() const
{
    return d->passed();
}

/** Return whether or not any of the tests have failed */
bool TestReportEditor::failed() const
{
    return d->failed();
}

/** Return whether or not this report is empty */
bool TestReportEditor::isEmpty() const
{
    return d->isEmpty();
}

/** Return the number of tests performed on this class */
int TestReportEditor::numberOfTests() const
{
    return d->numberOfTests();
}

/** Return the number of successfully passed tests */
int TestReportEditor::numberOfSuccesses() const
{
    return d->numberOfSuccesses();
}

/** Return the number of failed tests */
int TestReportEditor::numberOfFailures() const
{
    return d->numberOfFailures();
}

/** Return a report of all of the successful tests */
TestReport TestReportEditor::successes() const
{
    return d->successes();
}

/** Return a report of all of the failed tests */
TestReport TestReportEditor::failures() const
{
    return d->failures();
}

/** Return the class that was tested */
Class TestReportEditor::testedClass() const
{
    return d->clas;
}

/** Add the passed test report to this report */
TestReportEditor& TestReportEditor::addReport(const TestReport &report)
{
    d->items.append( report );
    return *this;
}

/** Test the passed boolean value, with the test passing if the value
    is true, but failing if the value is false */
TestReportEditor& TestReportEditor::testTrue(bool test, const String &description)
{
    if (test)
        d->items.append( TestPassedItem(description) );
    else
        d->items.append( TestFailedCompareItem(test, true, description) );

    return *this;
}

/** Test the passed boolean value, with the test passing if the value
    is false, but failing if the value is true */
TestReportEditor& TestReportEditor::testFalse(bool test, const String &description)
{
    if (test)
        d->items.append( TestFailedCompareItem(test, false, description) );
    else
        d->items.append( TestPassedItem(description) );

    return *this;
}

/** Add a passed report - this is used to just pass a test report, with 
    the passed reason */
TestReportEditor& TestReportEditor::addPassed(const String &description)
{
    d->items.append( TestPassedItem(description) );
    
    return *this;
}

/** Add a failed report - this is used to just fail a test report, with 
    the passed reason */
TestReportEditor& TestReportEditor::addFailure(const String &description)
{
    d->items.append( TestFailedItem(description) );

    return *this;
}

/** Add a failed report - this is used to just fail a test report, with 
    the passed reason */
TestReportEditor& TestReportEditor::addFailed(const String &description)
{
    return TestReportEditor::addFailure(description);
}

/** Add a captured exception to the report */
TestReportEditor& TestReportEditor::addException(const Exception &e)
{
    d->items.append( TestFailedExceptionItem(e) );

    return *this;
}

/** Add a captured exception to the report, with the passed description of the test */
TestReportEditor& TestReportEditor::addException(const Exception &e, 
                                                 const String &description)
{
    d->items.append( TestFailedExceptionItem(e,description) );

    return *this;
}

/** Test if the two passed objects are equal. The test passes if they are,
    but fails if they are not */
TestReportEditor& TestReportEditor::testObjectsEqual(const Object &obj0,
                                                     const Object &obj1,
                                                     const String &description)
{
    if (obj0 == obj1)
        d->items.append( TestPassedItem(description) );
    else
        d->items.append( TestFailedCompareItem(obj0, obj1, description) );

    return *this;
}
           
/** Test if the two passed integers are equal. This test passes if they are,
    but fails if they are not */
TestReportEditor& TestReportEditor::testIntegersEqual(int i0, int i1, 
                                                      const String &description)
{
    if (i0 == i1)
        d->items.append( TestPassedItem(description) );
    else
        d->items.append( TestFailedCompareItem(i0, i1, description) );

    return *this;
}

/** Test if the two passed floats are equal. This test passes if they are, 
    but failed if they are not. The floating point comparison uses the 
    (optionally supplied) tolerance to make the comparison */
TestReportEditor& TestReportEditor::testFloatsEqual(double i0, double i1,
                                                    const String &description, 
                                                    double tol)
{
    if (Siren::third_party::fcmp(i0, i1, tol) == 0)
        d->items.append( TestPassedItem(description) );
    else
        d->items.append( TestFailedCompareItem(i0, i1, tol, description) );

    return *this;
}

/** Test if the two passed objects are equal. This test passes if they are,
    but fails if they are not. */
TestReportEditor& TestReportEditor::testEqual(const Object &obj0, const Object &obj1,
                                              const String &description)
{
    this->testObjectsEqual(obj0, obj1, description);

    return *this;
}

/** Test if the two passed floats are equal. This test passes if they are, 
    but failed if they are not. The floating point comparison uses the 
    (optionally supplied) tolerance to make the comparison */
TestReportEditor& TestReportEditor::testEqual(double i0, double i1, 
                                              const String &description, double tol)
{
    this->testFloatsEqual(i0, i1, description, tol);

    return *this;
}
               
/** Test if the two passed integers are equal. This test passes if they are,
    but fails if they are not */
TestReportEditor& TestReportEditor::testEqual(int i0, int i1, const String &description)
{
    this->testIntegersEqual(i0, i1, description);

    return *this;
}
