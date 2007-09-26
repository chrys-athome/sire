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

#ifndef SIREMOL_RESIDENTIFIER_H
#define SIREMOL_RESIDENTIFIER_H

#include "SireID/identifier.h"

#include "resid.h"

namespace SireMol
{

template<class ID>
class Specify;

template<class ID>
class AtomsIn;

class ResIdentifier : public SireID::Identifier_T_<ResIdentifier,ResID>
{
public:
    ResIdentifier();
    ResIdentifier(const ResID &ResID);
    ResIdentifier(const ResIdentifier &other);
    
    ~ResIdentifier();

    Specify<ResIdentifier> operator[](int i) const;
    Specify<ResIdentifier> operator()(int i) const;
    Specify<ResIdentifier> operator()(int i, int j) const;
    
    AtomsIn<ResIdentifier> atom(int i) const;

    AtomsIn<ResIdentifier> atoms() const;
    AtomsIn<ResIdentifier> atoms(int i, int j) const;
    
    QList<ResIdx> map(const MoleculeInfoData &molinfo) const;
};

}

QDataStream& operator<<(QDataStream&, const SireMol::ResIdentifier&);
QDataStream& operator>>(QDataStream&, SireMol::ResIdentifier&);

XMLStream& operator<<(XMLStream&, const SireMol::ResIdentifier&);
XMLStream& operator>>(XMLStream&, SireMol::ResIdentifier&);

uint qHash(const SireMol::ResIdentifier &ResID);

Q_DECLARE_METATYPE(SireMol::ResIdentifier);

#endif

