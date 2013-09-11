#ifndef SIREN_PERCENTAGE_H
#define SIREN_PERCENTAGE_H

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
    /** This class represents a percentage (number out of 100).
        Only integer percentage values are supported
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Percentage : public Object
    {
        SIREN_CLASS(Percentage,Object,1)
    public:
        Percentage();
        Percentage(int percent);
        Percentage(const Percentage &other);
        
        ~Percentage();
        
        int value() const;
        
        operator int() const;
        
        String toString() const;
        
    protected:
        void copy_object(const Percentage &other);
        bool compare_object(const Percentage &other) const;
    
    private:
        /** The actual percentage, a number from 0 to 100 */
        uint8 percent;
    
    }; // end of class Siren::Percentage

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Percentage )

SIREN_END_HEADER

#endif // ifndef SIREN_PERCENTAGE_H
