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

#ifndef SIREDB_USING_RELATIONSHIPS_HPP
#define SIREDB_USING_RELATIONSHIPS_HPP

#include "using_database.h"

#include <QStringList>

SIRE_BEGIN_HEADER

namespace SireDB
{
class using_relationships_base;
}

QDataStream& operator<<(QDataStream&, const SireDB::using_relationships_base&);
QDataStream& operator>>(QDataStream&, SireDB::using_relationships_base&);

namespace SireDB
{

class SIREDB_EXPORT using_relationships_base : public using_database
{
public:
    /** Constructor */
    using_relationships_base() : using_database()
    {}
    
    /** Construct from a list of typenames */
    using_relationships_base(const QStringList &typenames)
                               : using_database(typenames)
    {}
    
    /** Copy constructor */
    using_relationships_base(const using_relationships_base &other)
                               : using_database(other)
    {}
    
    /** Destructor */
    ~using_relationships_base()
    {}
    
    using_relationships_base* clone() const
    {
        return new using_relationships_base(*this);
    }
    
    const char* what() const
    {
        return "SireDB::using_relationships_base";
    }
};

/** This class is used to specify which classes to use to 
    obtain relationships.
    
    You use this class by adding the databases to use onto
    the list of templates.
    
    @author Christopher Woods
*/
template<class T0, 
         class T1=detail::unspecified, class T2=detail::unspecified, 
         class T3=detail::unspecified, class T4=detail::unspecified, 
         class T5=detail::unspecified, class T6=detail::unspecified,
         class T7=detail::unspecified, class T8=detail::unspecified, 
         class T9=detail::unspecified>
class SIREDB_EXPORT using_relationships : public using_relationships_base
{
public:
    /** Constructor */
    using_relationships() : using_relationships_base()
    {
        //add the typename of each class in turn...
        this->addType( T0::typeName() );
        this->addType( T1::typeName() );
        this->addType( T2::typeName() );
        this->addType( T3::typeName() );
        this->addType( T4::typeName() );
        this->addType( T5::typeName() );
        this->addType( T6::typeName() );
        this->addType( T7::typeName() );
        this->addType( T8::typeName() );
        this->addType( T9::typeName() );
    }

    /** Copy constructor */
    using_relationships(const using_relationships &other) 
                        : using_relationships_base(other)
    {}
    
    /** Destructor */
    ~using_relationships()
    {}
    
private:
    /** Add the name of the type to the list */
    void addType(const char *type_name)
    {
        if (type_name)
            this->append( type_name );
    }
};

}

Q_DECLARE_METATYPE(SireDB::using_relationships_base)

SIRE_END_HEADER

#endif
