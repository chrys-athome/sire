/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "combinespaces.h"

#include "space.h"
#include "combinedspace.h"

#include "SireBase/properties.h"

#include "Siren/stream.h"

using namespace SireVol;
using namespace SireBase;
using namespace Siren;

static const RegisterObject<CombineSpaces> r_combinespaces;

/** Constructor */
CombineSpaces::CombineSpaces() : Implements<CombineSpaces,CombineProperties>()
{}

/** Construct to use just as single space, from the supplied source */
CombineSpaces::CombineSpaces(const PropertyName &source)
              : Implements<CombineSpaces,CombineProperties>(source)
{}

/** Construct to combine together the two spaces specified by the 
    two supplied sources */
CombineSpaces::CombineSpaces(const PropertyName &source0, const PropertyName &source1)
              : Implements<CombineSpaces,CombineProperties>(source0, source1)
{}

/** Construct to combine together the spaces from the passed sources */
CombineSpaces::CombineSpaces(const QList<PropertyName> &sources)
              : Implements<CombineSpaces,CombineProperties>(sources)
{}

/** Construct to combine together the spaces from the passed sources */
CombineSpaces::CombineSpaces(const QVector<PropertyName> &sources)
              : Implements<CombineSpaces,CombineProperties>(sources)
{}

/** Construct to combine together the spaces from the passed sources */
CombineSpaces::CombineSpaces(const QList<QString> &sources)
              : Implements<CombineSpaces,CombineProperties>(sources)
{}

/** Construct to combine together the spaces from the passed sources */
CombineSpaces::CombineSpaces(const QVector<QString> &sources)
              : Implements<CombineSpaces,CombineProperties>(sources)
{}

/** Copy constructor */
CombineSpaces::CombineSpaces(const CombineSpaces &other)
              : Implements<CombineSpaces,CombineProperties>(other)
{}

/** Destructor */
CombineSpaces::~CombineSpaces()
{}

/** Copy assignment operator */
CombineSpaces& CombineSpaces::operator=(const CombineSpaces &other)
{
    CombineProperties::operator=(other);
    return *this;
}

/** Comparison operator */
bool CombineSpaces::operator==(const CombineSpaces &other) const
{
    return CombineProperties::operator==(other);
}

/** Comparison operator */
bool CombineSpaces::operator!=(const CombineSpaces &other) const
{
    return CombineProperties::operator!=(other);
}

QString CombineSpaces::toString() const
{
    QStringList sources;
    
    for (CombineProperties::const_iterator it = this->constBegin();
         it != this->constEnd();
         ++it)
    {
        sources.append( it->toString() );
    }

    return QObject::tr("CombineSpaces( from [ %1 ] )")
                .arg(sources.join(", "));
}

void CombineSpaces::stream(Stream &s)
{
    s.assertVersion<CombineSpaces>(1);
    
    Schema schema = s.item<CombineSpaces>();
    
    CombineProperties::stream( schema.base() );
}

/** Return the combined space by extracting the required space 
    properties from 'properties'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
ObjRef CombineSpaces::combine(const Properties &properties) const
{
    if (this->isEmpty())
        return ObjRef();
        
    else if (this->count() == 1)
    {
        return properties.property(this->at(0)).asA<Space>();
    }
    else
    {
        QList<SpacePtr> spaces;
        
        for (CombineProperties::const_iterator it = this->constBegin();
             it != this->constEnd();
             ++it)
        {
            spaces.append( properties.property(*it).asA<Space>() );
        }
        
        return CombinedSpace(spaces);
    }
}
