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

using namespace Siren;

///////////
/////////// Implementation of Siren::TestReport
///////////

REGISTER_SIREN_CLASS( Siren::TestReport )

/** Constructor */
TestReport::TestReport() 
           : Object(), 
             Interfaces< Mutable<TestReportEditor,TestReport> >()
{}

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
             Interfaces< Mutable<TestReportEditor,TestReport> >(other)
{}

/** Destructor */
TestReport::~TestReport()
{}

/** Copy assignment operator */
void TestReport::copy_object(const TestReport &other)
{
    super::copy_object(other);
}

/** Comparison operator */
bool TestReport::compare_object(const TestReport &other) const
{
    return super::compare_object(other);
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
