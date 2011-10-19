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
#include "Siren/class.h"
#include "Siren/bytearray.h"
#include "Siren/stringlist.h"
#include "Siren/exception.h"
#include "Siren/obj.h"
#include "Siren/testreport.h"

using namespace Siren;

int main(int argc, const char **argv)
{
    TextStream cout(stdout);
    TextStream cerr(stderr);

    try
    {
        StringList classes = Class::registeredTypes();
        
        List<TestReport>::Type reports;
        
        for (StringList::const_iterator it = classes.constBegin();
             it != classes.constEnd();
             ++it)
        {
            Class c(*it);

            if (c.canCreate())
            {
                Obj obj = c.create();
                
                cout << "Testing " << obj.what() << "...";
                
                TestReport report = obj.test();
                
                if (report.passed())
                    cout << " (passed)\n";

                else
                {
                    cout << " (FAILED!)\n";
                    reports.append(report);
                }
                    
            }
        }
    }
    catch( const Siren::Exception &e )
    {
        cerr << e.toString() << "\n";
        return -1;
    }
    catch(...)
    {
        cerr << "Caught an unknown exception!\n";
        return -1;
    }
    
    return 0;
}
