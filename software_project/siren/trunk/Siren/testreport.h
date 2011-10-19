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
        TestReport(TestReportEditor &editor);
        
        TestReport(const TestReport &other);
        
        ~TestReport();
        
    protected:
        void copy_object(const TestReport &other);
        bool compare_object(const TestReport &other) const;
    
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
    

    }; // end of class TestReportEditor
}

SIREN_EXPOSE_CLASS( Siren::TestReport )

SIREN_END_HEADER

#endif // ifndef SIREN_TESTREPORT_H
