/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREDB_USING_DATABASE_H
#define SIREDB_USING_DATABASE_H

#include "assigninstruction.h"

#include <QStringList>

SIRE_BEGIN_HEADER

namespace SireDB
{
class using_database;
}

QDataStream& operator<<(QDataStream&, const SireDB::using_database&);
QDataStream& operator>>(QDataStream&, SireDB::using_database&);

namespace SireDB
{

/** This is the virtual base class of using_relationships and using_parameters.
    This class cannot be used on its own, but rather abstracts the 
    utilities of the two derived classes, and provides an interface for 
    the other assign classes.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT using_database : public AssignInstruction, public QStringList
{
public:
    /** Constructor */
    using_database() : AssignInstruction(), QStringList()
    {}
    
    /** Construct from a list of typenames */
    using_database(const QStringList &typenames) 
                    : AssignInstruction(), QStringList(typenames)
    {}
    
    /** Copy constructor */
    using_database(const using_database &other) 
                    : AssignInstruction(other), QStringList(other)
    {}
    
    /** Destructor */
    virtual ~using_database()
    {}

private:    
    /** Add a type onto the list */
    void addType(const char *type_name)
    {
        if (type_name)
            this->append(type_name);
    }
};

namespace detail
{
struct unspecified
{
    static const char* typeName()
    {
        return 0;
    }
};
}

}

SIRE_END_HEADER

#endif
