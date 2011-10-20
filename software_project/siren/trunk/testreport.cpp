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

using namespace Siren;

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
            
            TestReportData(const TestReportData &other)
                  : imp_shared_data(), 
                    clas(other.clas)
            {}
            
            ~TestReportData()
            {}
            
            /** The class being tested */
            Class clas;
            
        private:
            TestReportData& operator=(const TestReportData &other)
            {
                return *this;
            }
            
        }; // end of TestReportData
    
    } // end of namespace detail

} // end of namespace Siren

///////////
/////////// Implementation of Siren::TestReport
///////////

REGISTER_SIREN_CLASS( Siren::TestReport )

static imp_shared_ptr<detail::TestReportData> shared_null( 
                                                    new detail::TestReportData() );

/** Constructor */
TestReport::TestReport() 
           : Object(), 
             Interfaces< Mutable<TestReportEditor,TestReport> >(),
             d( shared_null )
{}

/** Construct to report on the tests of the passed class */
TestReport::TestReport(const Class &c)
           : Object(),
             Interfaces< Mutable<TestReportEditor,TestReport> >(),
             d( shared_null )
{
    d->clas = c;
}

/** Construct from the passed editor */
TestReport::TestReport(TestReportEditor &editor)
           : Object(),
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{
    this->operator=( editor.commit() );
}

/** Copy constructor */
TestReport::TestReport(const TestReport &other)
           : Object(other),
             Interfaces< Mutable<TestReportEditor,TestReport> >(other),
             d(other.d)
{}

/** Destructor */
TestReport::~TestReport()
{}

/** Return a string representation of the report */
String TestReport::toString() const
{
    return String::tr("Test report for the class %1.")
                .arg(cls.name());
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
    return false;
}

/** Return whether or not any of the tests failed */
bool TestReport::failed() const
{
    return true;
}

/** Return the class that was tested */
Class TestReport::testedClass() const
{
    return cls;
}

///////////
/////////// Implementation of Siren::TestReportEditor
///////////

/** Constructor */
TestReportEditor::TestReportEditor()
                 : Editor<TestReportEditor,TestReport>()
{}

/** Construct from the passed TestReport - this extracts the data
    from TestReport and packages it as an editable block */
TestReportEditor::TestReportEditor(const TestReport &report)
                 : Editor<TestReportEditor,TestReport>()
{
    //do stuff :-)
}

/** Copy constructor */
TestReportEditor::TestReportEditor(const TestReportEditor &other)
                 : Editor<TestReportEditor,TestReport>(other)
{}

/** Destructor */
TestReportEditor::~TestReportEditor()
{}

/** Commit the changes - this coverts the editable data back to read-only
    data and updates this editor that state. It then returns the read-only
    data as a TestReport object */
TestReport TestReportEditor::commit()
{
    return TestReport();
}
