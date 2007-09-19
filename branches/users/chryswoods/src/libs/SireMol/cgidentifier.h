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

#include "SireID/identifier.h"

#include "cgid.h"
#include "cgidx.h"

namespace SireMol
{

class CGIdentifier : public SireID::Identifier_T_<CGIdentifier,CGID>
{
public:
    CGIdentifier() : SireID::Identifier_T_<CGIdentifier,CGID>()
    {}
    
    CGIdentifier(const CGID &cgid) 
            : SireID::Identifier_T_<CGIdentifier,CGID>(cgid)
    {}
    
    CGIdentifier(const CGIdentifier &other) 
            : SireID::Identifier_T_<CGIdentifier,CGID>(other)
    {}
    
    ~CGIdentifier()
    {}
    
    CGIdx map(const MoleculeInfo &molinfo) const
    {
        this->assertNotNull();
        return this->asA<CGID>().map(molinfo);
    }
};

}

QDataStream& operator<<(QDataStream&, const SireMol::CGIdentifier&);
QDataStream& operator>>(QDataStream&, SireMol::CGIdentifier&);

XMLStream& operator<<(XMLStream&, const SireMol::CGIdentifier&);
XMLStream& operator>>(XMLStream&, SireMol::CGIdentifier&);

uint qHash(const SireMol::CGIdentifier &cgid);

Q_DECLARE_METATYPE(SireMol::CGIdentifier);

#endif

