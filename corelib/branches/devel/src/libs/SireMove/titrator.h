/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#ifndef SIREMOVE_TITRATOR_H
#define SIREMOVE_TITRATOR_H

#include "SireBase/property.h"
#include "SireMol/mgname.h"

#include "SireMol/atomcharges.h"
#include "SireMM/atomljs.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Titrator;
}

QDataStream& operator<<(QDataStream&, const SireMove::Titrator&);
QDataStream& operator>>(QDataStream&, SireMove::Titrator&);

namespace SireSystem{ class System; }

namespace SireMove
{

using SireSystem::System;

/** This property is used by the TitrationMove to maintain a list
    of which molecules can be titrated, and the list of titration
    states of each molecule.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT Titrator
        : public SireBase::ConcreteProperty<Titrator,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Titrator&);
friend QDataStream& ::operator>>(QDataStream&, Titrator&);

public:
    Titrator();
    Titrator(const Titrator &other);
    
    ~Titrator();
    
    static const char* typeName();
    const char* what() const;
    
    Titrator& operator=(const Titrator &other);
    
    bool operator==(const Titrator &other) const;
    bool operator!=(const Titrator &other) const;

    void setPositiveIonParameters(const SireMol::AtomCharges &chgs,
                                  const SireMM::AtomLJs &ljs);
    
    void setNegativeIonParameters(const SireMol::AtomCharges &chgs,
                                  const SireMM::AtomLJs &ljs);
    
    void setNeutralParameters(const SireMol::AtomCharges &chgs,
                              const SireMM::AtomLJs &ljs);

    int nPositiveIons() const;
    int nNeutrals() const;
    int nNegativeIons() const;
    
    void swapPositiveIon(int neutral_index, int ion_index);
    void swapNegativeIon(int neutral_index, int ion_index);
    
    double applyTo(System &system) const;

private:
    /** The name of the molecule group containing all of the 
        molecules that can swap */
    SireMol::MGName mgname;

    /** The current charge of each molecule in the group */
    QVector<qint32> chgs;
    
    /** The desired charge of each molecule in the group */
    QVector<qint32> desired_chgs;
    
    /** The charge parameters for positive ions */
    SireMol::AtomCharges pos_chgs;
    
    /** The charge parameters for negative ions */
    SireMol::AtomCharges neg_chgs;
    
    /** The charge parameters for neutral molecules */
    SireMol::AtomCharges neut_chgs;
    
    /** The LJ parameters for positive ions */
    SireMM::AtomLJs pos_ljs;
    
    /** The LJ parameters for negative ions */
    SireMM::AtomLJs neg_ljs;
    
    /** The LJ parameters for neutral molecules */
    SireMM::AtomLJs neut_ljs;
    
    /** Map to locate the properties used by this Titrator */
    SireBase::PropertyMap propmap;
};

} // end of namespace SireMove

Q_DECLARE_METATYPE( SireMove::Titrator )

SIRE_EXPOSE_CLASS( SireMove::Titrator )

SIRE_END_HEADER

#endif