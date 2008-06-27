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

#ifndef SQUIRE_EFF_H
#define SQUIRE_EFF_H

#include "SireFF/g1ff.h"
#include "SireFF/ff3d.h"

#include "effcomponents.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class EFF;
}

QDataStream& operator<<(QDataStream&, const Squire::EFF&);
QDataStream& operator>>(QDataStream&, Squire::EFF&);

namespace Squire
{

using SireBase::PropertyMap;

using SireFF::ForceField;

using SireMol::PartialMolecule;
using SireMol::Molecule;
using SireMol::MolNum;

/** Implementation of the "electron forcefield" model proposed
    by Julius Su and William A. Goddard III. This model represents
    each electron as a gaussian wave packet, and solves the 
    equations of motion for each electron individually
    (to find the minimum energy configuration for a given
    set of nuclear coordinates)
    
    See www.electronforcefield.com
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT EFF : public SireBase::ConcreteProperty<EFF, SireFF::G1FF>,
                          public SireFF::FF3D
{

friend QDataStream& ::operator<<(QDataStream&, const EFF&);
friend QDataStream& ::operator>>(QDataStream&, EFF&);

public:
    typedef EFFComponents Components;

    EFF();
    EFF(const QString &name);
    
    EFF(const EFF &other);
    
    ~EFF();

    EFF& operator=(const EFF &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<EFF>() );
    }
    
    const char* what() const
    {
        return EFF::typeName();
    }
    
    bool operator==(const EFF &other) const;
    bool operator!=(const EFF &other) const;
    
    EFF* clone() const
    {
        return new EFF(*this);
    }

    const Components& components() const;

    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;

    void mustNowRecalculateFromScratch();    
    
    void force(ForceTable &forcetable, double scale_force=1);
    void force(ForceTable &forcetable, const SireCAS::Symbol &component,
               double scale_force=1);
    
protected:
    void recalculateEnergy();

    void _pvt_restore(const ForceField &ffield);
    
    void _pvt_added(const PartialMolecule &mol, const PropertyMap &map);

    void _pvt_removed(const PartialMolecule &mol);

    void _pvt_changed(const SireMol::Molecule &molecule);
    void _pvt_changed(const QList<SireMol::Molecule> &molecules);
    
    void _pvt_removedAll();
        
    bool _pvt_wouldChangeProperties(MolNum molnum, 
                                    const PropertyMap &map) const;

    void _pvt_updateName();

private:
    
};

}

SIRE_EXPOSE_CLASS( Squire::EFF )

Q_DECLARE_METATYPE( Squire::EFF );

SIRE_END_HEADER

#endif
