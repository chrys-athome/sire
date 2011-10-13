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

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This class holds a report of the results of unit testing
        of a Siren::Object class. Each Siren::Object class provides
        in-built unit testing, to ensure that it is functioning 
        correctly, and to encourage developers to add test code 
        directly into the source.

        @author Christopher Woods
    */
    class SIREN_EXPORT TestReport 
            : public Object, public Interfaces<
    {
        SIREN_CLASS( TestReport, Object )
        
    
    }; // end of class TestReport


}

SIREN_EXPOSE_CLASS( Siren::TestReport )

SIREN_END_HEADER

#endif // ifndef SIREN_TESTREPORT_H
