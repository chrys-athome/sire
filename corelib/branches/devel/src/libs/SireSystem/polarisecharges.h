/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIRESYSTEM_POLARISECHARGES_H
#define SIRESYSTEM_POLARISECHARGES_H

#include "chargeconstraint.h"

#include "SireFF/probe.h"
#include "SireCAS/symbol.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class PolariseCharges;
}

QDataStream& operator<<(QDataStream&, const SireSystem::PolariseCharges&);
QDataStream& operator>>(QDataStream&, SireSystem::PolariseCharges&);

namespace SireSystem
{

namespace detail
{
class PolariseChargesData;
}

/** This charge constraint adjusts the partial charges of contained
    molecules to give the impression that the molecule contains
    polarisable dipoles. This is based on the method developed
    by Reynolds et al. (see ...)
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT PolariseCharges
         : public SireBase::ConcreteProperty<PolariseCharges,ChargeConstraint>
{

friend QDataStream& ::operator<<(QDataStream&, const PolariseCharges&);
friend QDataStream& ::operator>>(QDataStream&, PolariseCharges&);

public:
    PolariseCharges();
    PolariseCharges(const MoleculeGroup &molgroup,
                    const PropertyMap &map = PropertyMap());
    PolariseCharges(const MoleculeGroup &molgroup,
                    const SireFF::Probe &probe,
                    const PropertyMap &map = PropertyMap());

    PolariseCharges(const MoleculeGroup &molgroup,
                    const SireCAS::Symbol &field_component,
                    const PropertyMap &map = PropertyMap());
    PolariseCharges(const MoleculeGroup &molgroup,
                    const SireCAS::Symbol &field_component,
                    const SireFF::Probe &probe,
                    const PropertyMap &map = PropertyMap());
    
    PolariseCharges(const PolariseCharges &other);
    
    ~PolariseCharges();
    
    PolariseCharges& operator=(const PolariseCharges &other);
    
    bool operator==(const PolariseCharges &other) const;
    bool operator!=(const PolariseCharges &other) const;
    
    static const char* typeName();
    
    PolariseCharges* clone() const;
    
    QString toString() const;

    const SireCAS::Symbol& fieldComponent() const;

    const SireFF::Probe& probe() const;

protected:
    void setSystem(const System &system);

    bool mayChange(const Delta &delta, quint32 last_subversion) const;
    
    bool fullApply(Delta &delta);
    bool deltaApply(Delta &delta, quint32 last_subversion);

private:
    void setProbe(const SireFF::Probe &probe);

    /** The forcefield component that is used to calculate 
        the potential on the atoms to be polarised */
    SireCAS::Symbol field_component;
    
    /** The probe used to calculate the potential on the atoms */
    SireFF::ProbePtr field_probe;
    
    /** The charge on the probe (in mod_electrons) */
    double probe_charge;

    /** The information about each molecule that is needed
        to calculate the polarisability */
    QHash<SireMol::MolNum,detail::PolariseChargesData> moldata;
};

}

Q_DECLARE_METATYPE( SireSystem::PolariseCharges )

SIRE_EXPOSE_CLASS( SireSystem::PolariseCharges )

SIRE_END_HEADER

#endif
