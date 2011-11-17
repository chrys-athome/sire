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
#include "Siren/siren.hpp"
#include "Siren/class.h"
#include "Siren/bytearray.h"
#include "Siren/stringlist.h"
#include "Siren/exception.h"
#include "Siren/exceptions.h"
#include "Siren/obj.h"
#include "Siren/object.hpp"
#include "Siren/testreport.h"
#include "Siren/obj.hpp"

using namespace Siren;

int main(int argc, const char **argv)
{
    Siren::init(argc, argv);

    TextStream cout(stdout);
    TextStream cerr(stderr);

    TestReport report( String::tr("Testing all libSiren classes") );
    TestReport::Editor editor = report.edit();
        
    StringList classes = Class::registeredTypes();

    for (StringList::const_iterator it = classes.constBegin();
         it != classes.constEnd();
         ++it)
    {
        Class tested_class(*it);

        try
        {
            if (tested_class.canCreate())
            {
                Obj obj = tested_class.create();
                
                cout << "Testing " << obj.what() << "...";
                cout.flush();
                
                TestReport report = obj.test();
                
                editor.addReport(report);
                
                if (report.isEmpty())
                    cout << " (no tests)\n";
        
                else if (report.passed())
                    cout << " (passed)\n";

                else
                {
                    cout << " (FAILED!)\n";
                }
                    
            }
        }
        catch( const Siren::Exception &e )
        {
            editor.addException(e, 
                    String::tr("Exception caught while testing class \"%1\".")
                            .arg(tested_class.name()) );
        }
        catch( const std::exception &e )
        {
            editor.addException( standard_exception(e, CODELOC),
                    String::tr("Exception caught while testing class \"%1\".")
                            .arg(tested_class.name()) );
        }
        catch(...)
        {
            editor.addException( unknown_exception(CODELOC),
                    String::tr("Exception caught while testing class \"%1\".")
                            .arg(tested_class.name()) );
        }
    }
    
    report = editor.commit();
    
    cout << "\n\n DETAILED TEST REPORT\n\n" << report.toString() << "\n";
    
    List<String>::Type missing_tests;
    
    for (int i=0; i<report.count(); ++i)
    {
        TestReport class_report = report[i].asA<TestReport>();

        if (class_report.failed())
        {
            cout << (i+1) << " : " << class_report.toString() << "\n";
        
            for (int j=0; j<class_report.count(); ++j)
            {
                cout << "    " << (i+1) << "." << (j+1) << " : "
                     << class_report[j].toString() << "\n";
            }
        }
        else if (class_report.isEmpty())
        {
            missing_tests.append( class_report.testedClass().name() );
        }
    }
    
    if (not missing_tests.isEmpty())
    {
        cout << "\nWARNING: Some classes are missing unit tests:\n["
             << StringList(missing_tests).join(", ") << "]\n";
    }
    
    if (report.passed())
        return 0;
        
    else
        return -1;
}
