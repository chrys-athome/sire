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
#include "Siren/editor.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    class TestReportEditor;

    namespace detail
    {
        class TestReportData;
    }

    /** This class holds a report of the results of unit testing
        of a Siren::Object class. Each Siren::Object class provides
        in-built unit testing, to ensure that it is functioning 
        correctly, and to encourage developers to add test code 
        directly into the source.

        @author Christopher Woods
    */
    class SIREN_EXPORT TestReport 
            : public Object, public Interfaces< Mutable<TestReportEditor,
                                                        TestReport> >
    {
        SIREN_CLASS( TestReport, Object )
    
    public:
        TestReport();
        
        TestReport(const Class &c);
        
        TestReport(TestReportEditor &editor);
        
        TestReport(const TestReport &other);
        
        ~TestReport();
        
        String toString() const;
        
        bool passed() const;
        bool failed() const;
        
        int numberOfTests() const;
        int numberOfSuccesses() const;
        int numberOfFailures() const;
        
        StringList successes() const;
        StringList failures() const;
        
        Class testedClass() const;
        
    protected:
        void copy_object(const TestReport &other);
        bool compare_object(const TestReport &other) const;

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
        
        TestReport commit();
    
        void startGroup(const String &description);
    
        void testTrue(bool test, const String &description);
        void testFalse(bool test, const String &description);

        void addFailure(const String &description);

        void addException(const Exception &e);
        void addException(const std::exception &e);
        void addUnknownException();
                
        void testObjectsEqual(const Object &obj0, const Object &obj1,
                              const String &description);
                                     
        void testIntegersEqual(int i0, int i1, 
                              const String &description);
                              
        void testFloatsEqual(double i0, double i1,
                             const String &description,
                             double tol=1.0e-6);

        template<class T>
        void testEqual(const T &obj0, const T &obj1,
                       const String &description);

    private:
        imp_shared_ptr<detail::TestReportData>::Type d;

    }; // end of class TestReportEditor


    template<>
    SIREN_INLINE_TEMPLATE
    void TestReportEditor::testEqual<int>(const int &obj0, const int &obj1,
                                          const String &description)
    {
        this->testIntegersEqual(obj0, obj1, description);
    }
    
    template<>
    SIREN_INLINE_TEMPLATE
    void TestReportEditor::testEqual<double>(const double &obj0, const double &obj1,
                                             const String &description)
    {
        this->testFloatsEqual(obj0, obj1, description);
    }
    
    template<>
    SIREN_INLINE_TEMPLATE
    void TestReportEditor::testEqual<Object>(const Object &obj0, const Object &obj1,
                                             const String &description)
    {
        this->testObjectsEqual(obj0, obj1, description);
    }
}

SIREN_EXPOSE_CLASS( Siren::TestReport )
SIREN_EXPOSE_CLASS( Siren::TestReportEditor )

SIREN_END_HEADER

#endif // ifndef SIREN_TESTREPORT_H
