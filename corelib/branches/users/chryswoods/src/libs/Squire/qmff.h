/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SQUIRE_QMFF_H
#define SQUIRE_QMFF_H

#include "qmpotential.h"

#include "SireFF/g1ff.h"
#include "SireFF/ff3d.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class QMFF;
}

QDataStream& operator<<(QDataStream&, const Squire::QMFF&);
QDataStream& operator>>(QDataStream&, Squire::QMFF&);

namespace Squire
{

/** This is a forcefield that uses an external Quantum Chemical program
    to calculate the quantum mechanics energy and / or force on the
    contained molecules.
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT QMFF : public SireBase::ConcreteProperty<QMFF,SireFF::G1FF>,
                           public SireFF:FF3D,
                           protected QMPotential
{

friend QDataStream& ::operator<<(QDataStream&, const QMFF&);
friend QDataStream& ::operator>>(QDataStream&, QMFF&);

public:
    QMFF();
    QMFF(const QString &name);
    
    QMFF(const QMFF &other);
    
    ~QMFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<QMFF>() );
    }
    
    const char* what() const
    {
        return QMFF::typeName();
    }
    
    QMFF& operator=(const QMFF &other);
    
    bool operator==(const QMFF &other) const;
    bool operator!=(const QMFF &other) const;
    
    const Components& components() const;

    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;

    void mustNowRecalculateFromScratch();    

    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);
};

}

Q_DECLARE_METATYPE( Squire::QMFF );

SIRE_EXPORT_CLASS( Squire::QMFF )

SIRE_END_HEADER

#endif
