/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREID_ID_H
#define SIREID_ID_H

#include "sireglobal.h"

#include "Siren/object.h"

#include <QString>
#include <QMetaType>

SIRE_BEGIN_HEADER

namespace SireID
{

/** This is the base class of all ID objects. An ID object
    is an object that is used to identify another object.
    
    @author Christopher Woods
*/
class SIREID_EXPORT ID : public Siren::Extends<ID,Siren::Object>
{
public:
    ID();
    ID(const ID&);
    
    virtual ~ID();
    
    static QString typeName();

    void stream(Siren::Stream &s);
 
    /** Return whether or not this ID is null */
    virtual bool isNull() const=0;

protected:
    ID& operator=(const ID&);
};

}

SIRE_EXPOSE_CLASS( SireID::ID )

SIRE_END_HEADER

#endif
