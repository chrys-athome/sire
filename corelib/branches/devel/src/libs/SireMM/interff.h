/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#ifndef SIREMM_INTERFF_H
#define SIREMM_INTERFF_H

#include "cljcomponent.h"
#include "cljgrid.h"
#include "cljfunction.h"

#include "SireFF/g1ff.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterFF&);

namespace SireMM
{

using SireBase::Property;
using SireBase::Properties;

/** This is a forcefield that calculates the intermolecular coulomb
    and Lennard Jones (LJ) energy of all contained molecule views.
    It also calculates the interactions with any fixed atoms added
    to this forcefield
    
    @author Christopher Woods
*/
class SIREMM_EXPORT InterFF : public SireBase::ConcreteProperty<InterFF,SireFF::G1FF>
{

friend QDataStream& ::operator<<(QDataStream&, const InterFF&);
friend QDataStream& ::operator>>(QDataStream&, InterFF&);

public:
    InterFF();
    InterFF(const QString &name);
    
    InterFF(const InterFF &other);
    
    ~InterFF();
    
    static const char* typeName();
    
    const char* what() const;
    
    InterFF& operator=(const InterFF &other);
    
    bool operator==(const InterFF &other) const;
    bool operator!=(const InterFF &other) const;

    InterFF* clone() const;

    const CLJComponent& components() const;

    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;

    void mustNowRecalculateFromScratch();    

private:
    void recalculateEnergy();
    
    void _pvt_added(const SireMol::PartialMolecule &mol,
                    const SireBase::PropertyMap &map);

    void _pvt_removed(const SireMol::PartialMolecule &mol);

    void _pvt_changed(const SireMol::Molecule &molecule);
    void _pvt_changed(const QList<SireMol::Molecule> &molecules);
    
    void _pvt_removedAll();
        
    bool _pvt_wouldChangeProperties(SireMol::MolNum molnum,
                                    const SireBase::PropertyMap &map) const;

    void _pvt_updateName();

    /** The function used to calculate energies */
    CLJFunctionPtr cljfunc;

    /** All of the boxed-up atoms */
    CLJBoxes cljboxes;
    
    /** All of the fixed atoms */
    CLJGrid fixed_atoms;

    /** The energy components available for this forcefield */
    CLJComponent cljcomps;
    
    /** All of the properties in this forcefield */
    Properties props;
};

}

Q_DECLARE_METATYPE( SireMM::InterFF )

SIRE_EXPOSE_CLASS( SireMM::InterFF )

SIRE_END_HEADER

#endif
