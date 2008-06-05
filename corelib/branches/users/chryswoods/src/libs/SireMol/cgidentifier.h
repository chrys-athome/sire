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

#ifndef SIREMOL_CGIDENTIFIER_H
#define SIREMOL_CGIDENTIFIER_H

#include "cgid.h"

#include <boost/shared_ptr.hpp>

namespace SireMol
{

class CGIdx;

class CGIdentifier : public CGID
{
public:
    CGIdentifier();
    CGIdentifier(const CGID &cgid);
    CGIdentifier(const CGIdentifier &other);
    
    ~CGIdentifier();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CGIdentifier>() );
    }
    
    const char* what() const
    {
        return CGIdentifier::typeName();
    }
    
    CGIdentifier* clone() const
    {
        return new CGIdentifier(*this);
    }
    
    bool isNull() const;
    
    uint hash() const;
                
    QString toString() const;
    
    const CGID& base() const;
    
    CGIdentifier& operator=(const CGIdentifier &other);
    CGIdentifier& operator=(const CGID &other);
    
    bool operator==(const SireID::ID &other) const;
    using SireID::ID::operator!=;
   
    bool operator==(const CGIdentifier &other) const;
    bool operator!=(const CGIdentifier &other) const;
    
    bool operator==(const CGID &other) const;
    bool operator!=(const CGID &other) const;
    
    QList<CGIdx> map(const MolInfo &molinfo) const;

private:
    /** Pointer to the CGID */
    boost::shared_ptr<CGID> d;
};

}

Q_DECLARE_METATYPE(SireMol::CGIdentifier);

#endif

