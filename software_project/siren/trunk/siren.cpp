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
#include "Siren/exceptions.h"
#include "Siren/static.h"
#include "Siren/forages.h"

namespace Siren
{
    /** Wrap the index 'i' so that it indexes into a container
        of 'count' elements. If the index >= 0, then it counts
        forward from the start, while if it is < 0 then it counts
        back from the last item in the container
        
        \throw Siren::invalid_index
    */
    int SIREN_EXPORT wrap_index(int i, int count)
    {
        if (i >= count or i <= -count)
            throw Siren::invalid_index( String::tr(
                    "Invalid index \"%1\" for a container with size \"%2\".")
                        .arg(i).arg(count), CODELOC );
                        
        if (i >= 0)
            return i;
        else
            return count - i;
    }

    /** Wrap the index 'i' so that it indexes into a container
        of 'count' elements. If the index >= 0, then it counts
        forward from the start, while if it is < 0 then it counts
        back from the last item in the container
        
        \throw Siren::invalid_index
    */
    int SIREN_EXPORT wrap_index(int i, int count, CODELOC_ARGS)
    {
        if (i >= count or i <= -count)
            throw Siren::invalid_index( String::tr(
                    "Invalid index \"%1\" for a container with size \"%2\".")
                        .arg(i).arg(count), CODELOC_PASS_ARGS );
                        
        if (i >= 0)
            return i;
        else
            return count - i;
    }

    namespace detail
    {
        void fini()
        {
            //send the end of for_ages
            for_ages::end();

            //now delete all of the static objects
            Siren::detail::Static::deleteAll();
        }
    }

    /** Call this once before using any Siren classes to ensure that 
        all Siren libraries are initialised */
    void SIREN_EXPORT init(int argc, const char **argv)
    {
        Siren::detail::Static::createAll();
        
        if (std::atexit(&(Siren::detail::fini)) != 0)
        {
            sirenDebug() << "CANNOT REGISTER EXIT HANDLER!!!";
            std::exit(-1);
        }
    }
    
    /** Call this once at the end of your program to ensure that 
        all of the Siren libraries are closed down properly */
    void SIREN_EXPORT fini()
    {
        Siren::detail::fini();
    }
    
    void SIREN_EXPORT loadLibrary(const char *library)
    {
        sirenDebug() << "Loading library" << library << "...";
        
        //find the library
        
        //load the library
        
        //resolve symbols - this will find all of the Siren object metadata
        
        //create all static objects - this initialises the library
        Siren::detail::Static::createAll();
    }


} // end of namespace Siren
