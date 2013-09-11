#ifndef SIREN_TESTREPORT_H
#define SIREN_TESTREPORT_H
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

#include "Siren/siren.h"
#include "Siren/interfaces.h"
#include "Siren/mutable.h"
#include "Siren/mutable.hpp"
#include "Siren/editor.h"
#include "Siren/obj.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    class TestReportEditor;

    namespace detail
    {
        class TestReportData;
    }

    /** This is the base class of all test report items. These
        are the details and results on an individual test */
    class SIREN_EXPORT TestReportItem : public Object
    {
        SIREN_VIRTUAL_CLASS(TestReportItem,Object,1)
    
    public:
        TestReportItem();
        TestReportItem(const Qt::String &description);
        
        TestReportItem(const TestReportItem &other);
        
        ~TestReportItem();
        
        const Qt::String& description() const;
        
        virtual bool passed() const=0;
        bool failed() const;
        
    protected:
        void copy_object(const TestReportItem &other);
        bool compare_object(const TestReportItem &other) const;
        
    private:
        /** The description of the test */
        Qt::String desc;
        
    }; // end of TestReportItem
    
    /** This test report item is used to store the fact that a test passed */
    class SIREN_EXPORT TestPassedItem : public TestReportItem
    {
        SIREN_CLASS(TestPassedItem,TestReportItem,1)
        
    public:
        TestPassedItem();
        TestPassedItem(const Qt::String &description);
        
        TestPassedItem(const TestPassedItem &other);
        
        ~TestPassedItem();
        
        bool passed() const;
        
        Qt::String toString() const;
        
    protected:
        void copy_object(const TestPassedItem &other);
        bool compare_object(const TestPassedItem &other) const;
        
    }; // end of class TestPassedItem
    
    /** This is the test report item that holds a simple error message */
    class SIREN_EXPORT TestFailedItem : public TestReportItem
    {
        SIREN_CLASS(TestFailedItem,TestReportItem,1)
    
    public:
        TestFailedItem();
        TestFailedItem(const Qt::String &description);
        
        TestFailedItem(const TestFailedItem &other);
        
        ~TestFailedItem();
        
        bool passed() const;
        
        Qt::String toString() const;
        
    protected:
        void copy_object(const TestFailedItem &other);
        bool compare_object(const TestFailedItem &other) const;
    
    }; // end of class TestFailedItem
    
    /** This is the test report item that holds the report of a failed
        comparison test */
    class SIREN_EXPORT TestFailedCompareItem : public TestReportItem
    {
        SIREN_CLASS(TestFailedCompareItem,TestReportItem,1)
        
    public:
        TestFailedCompareItem();
        TestFailedCompareItem(bool result, bool expected,
                              const Qt::String &description);
                              
        TestFailedCompareItem(int result, int expected,
                              const Qt::String &description);
                              
        TestFailedCompareItem(double result, double expected,
                              double tolerance, const Qt::String &description);
                              
        TestFailedCompareItem(const Object &result, const Object &expected,
                              const Qt::String &description);
                              
        TestFailedCompareItem(const TestFailedCompareItem &other);
        
        ~TestFailedCompareItem();
        
        bool passed() const;
        
        bool isBooleanComparison() const;
        bool isNumberComparison() const;
        bool isObjectComparison() const;
        
        const Object& expected() const;
        const Object& result() const;
        
        double tolerance() const;
        
        Qt::String toString() const;
        
    protected:
        void copy_object(const TestFailedCompareItem &other);
        bool compare_object(const TestFailedCompareItem &other) const;
        
    private:
        /** The expected value for the test */
        Obj expected_val;
        
        /** The actual, obtained value */
        Obj obtained_val;
        
        /** The tolerance (if comparing doubles) */
        double tol;
        
    }; // end of class TestFailedCompareItem
    
    /** This is the test report item that holds any exceptions raised
        during the execution of a test */
    class SIREN_EXPORT TestFailedExceptionItem : public TestReportItem
    {
        SIREN_CLASS(TestFailedExceptionItem,TestReportItem,1)
    
    public:
        TestFailedExceptionItem();
        TestFailedExceptionItem(const Exception &e);
        TestFailedExceptionItem(const Exception &e, const Qt::String &description);
        
        TestFailedExceptionItem(const TestFailedExceptionItem &other);
        
        ~TestFailedExceptionItem();
        
        bool passed() const;
    
        const Exception& exception() const;
    
        Qt::String toString() const;
    
    protected:
        void copy_object(const TestFailedExceptionItem &other);
        bool compare_object(const TestFailedExceptionItem &other) const;
        
    private:
        /** The exception thrown */
        Obj excptn;
    
    }; // end of class TestFailedExceptionItem
    
    /** This class holds a report of the results of unit testing
        of a Siren::Object class. Each Siren::Object class provides
        in-built unit testing, to ensure that it is functioning 
        correctly, and to encourage developers to add test code 
        directly into the source.

        @author Christopher Woods
    */
    class SIREN_EXPORT TestReport 
            : public TestReportItem, public Interfaces< Mutable<TestReportEditor,
                                                                TestReport> >
    {
        SIREN_CLASS(TestReport,TestReportItem,1)
    
    public:
        TestReport();
        
        TestReport(const Qt::String &description);
        TestReport(const Class &c);
        TestReport(const Class &c, const Qt::String &description);
        
        TestReport(TestReportEditor &editor);
        
        TestReport(const TestReport &other);
        
        ~TestReport();
        
        Obj operator[](int i) const;
        Obj at(int i) const;
        
        int count() const;
        int size() const;
        
        Qt::String toString() const;
        
        bool passed() const;
        bool failed() const;
        
        bool isEmpty() const;
        
        int numberOfTests() const;
        int numberOfSuccesses() const;
        int numberOfFailures() const;
        
        TestReport successes() const;
        TestReport failures() const;
        
        Class testedClass() const;
        
    protected:
        void copy_object(const TestReport &other);
        bool compare_object(const TestReport &other) const;

        void test(TestReportEditor &report) const;
        void stressTest(TestReportEditor &report) const;

    private:
        friend class TestReportEditor;
        imp_shared_ptr<detail::TestReportData>::Type d;
    
    }; // end of class TestReport


    /** This is the class that is used to write and edit
        a TestReport
        
        @author Christopher Woods
    */
    class SIREN_EXPORT TestReportEditor 
            : public Editor<TestReportEditor,TestReport>
    {
    public:
        TestReportEditor();
        TestReportEditor(const TestReport &report);
        TestReportEditor(const TestReportEditor &other);
        
        ~TestReportEditor();
        
        TestReportEditor& operator=(const TestReportEditor &other);
        
        bool operator==(const TestReportEditor &other) const;
        
        Obj operator[](int i) const;
        Obj at(int i) const;
        
        int count() const;
        int size() const;
        
        const Qt::String& description() const;
        
        Qt::String toString() const;
        
        bool passed() const;
        bool failed() const;
        
        bool isEmpty() const;
        
        int numberOfTests() const;
        int numberOfSuccesses() const;
        int numberOfFailures() const;
        
        TestReport successes() const;
        TestReport failures() const;
        
        Class testedClass() const;
        
        TestReport commit();

        TestReportEditor& addReport(const TestReport &report);
    
        TestReportEditor& testTrue(bool test, const Qt::String &description);
        TestReportEditor& testFalse(bool test, const Qt::String &description);

        TestReportEditor& addPassed(const Qt::String &description);
        TestReportEditor& addFailed(const Qt::String &description);
        TestReportEditor& addFailure(const Qt::String &description);

        TestReportEditor& addException(const Exception &e);
        TestReportEditor& addException(const Exception &e, const Qt::String &description);

        TestReportEditor& testObjectsEqual(const Object &obj0, const Object &obj1,
                                           const Qt::String &description);
                                     
        TestReportEditor& testIntegersEqual(int i0, int i1, 
                                            const Qt::String &description);
                              
        TestReportEditor& testFloatsEqual(double i0, double i1,
                                          const Qt::String &description,
                                          double tol=1.0e-6);

        TestReportEditor& testEqual(const Object &obj0, const Object &obj1,
                                    const Qt::String &description);
                       
        TestReportEditor& testEqual(double i0, double i1, 
                                    const Qt::String &description, double tol=1.0e-6);
                       
        TestReportEditor& testEqual(int i0, int i1, const Qt::String &description);

    private:
        imp_shared_ptr<detail::TestReportData>::Type d;

        /** A description of the report */
        Qt::String desc;

    }; // end of class TestReportEditor
}

SIREN_EXPOSE_CLASS( Siren::TestReportItem )
SIREN_EXPOSE_CLASS( Siren::TestReport )
SIREN_EXPOSE_CLASS( Siren::TestReportEditor )

SIREN_EXPOSE_CLASS( Siren::TestPassedItem )
SIREN_EXPOSE_CLASS( Siren::TestFailedItem )
SIREN_EXPOSE_CLASS( Siren::TestFailedCompareItem )
SIREN_EXPOSE_CLASS( Siren::TestFailedExceptionItem )

SIREN_END_HEADER

#endif // ifndef SIREN_TESTREPORT_H
