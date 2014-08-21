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

#ifndef SIREMM_CLJ14GROUP_H
#define SIREMM_CLJ14GROUP_H

#include "twoatomfunctions.h"

#include "SireMol/partialmolecule.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJ14Group;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJ14Group&);
QDataStream& operator>>(QDataStream&, SireMM::CLJ14Group&);

namespace SireMM
{

namespace detail
{
class CLJ14AtomPair;
}

using SireMol::PartialMolecule;
using SireMol::MoleculeView;
using SireMol::CGIdx;

using SireBase::PropertyMap;

/** This class holds all of the information needed to calculate
    the 14-nonbonded energy for a molecule
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJ14Group
{

friend QDataStream& ::operator<<(QDataStream&, const CLJ14Group&);
friend QDataStream& ::operator>>(QDataStream&, CLJ14Group&);

public:
    CLJ14Group();
    CLJ14Group(const MoleculeView &molecule, const PropertyMap &map = PropertyMap());
    CLJ14Group(const CLJ14Group &other);
    
    ~CLJ14Group();
    
    CLJ14Group& operator=(const CLJ14Group &other);
    
    bool operator==(const CLJ14Group &other) const;
    bool operator!=(const CLJ14Group &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    QString toString() const;
    
    bool isNull() const;
    
    const MoleculeView& molecule() const;
    PropertyMap propertyMap() const;

    void add(const MoleculeView &new_molecule);
    void add(const AtomSelection &new_selection);
    
    void updateSelection(const AtomSelection &selection);
    
    void update(const MoleculeView &new_molecule);

    void remove(const AtomSelection &new_selection);
    void remove(const MoleculeView &new_molecule);
    
    boost::tuple<double,double> energy();

    bool recalculatingFromScratch() const;
    void mustNowRecalculateFromScratch();
    void mustReallyRecalculateFromScratch();
    
private:
    /** The molecule whose 14 energy is being calculated */
    PartialMolecule mol;
    
    /** The new molecule if we have been updated */
    PartialMolecule newmol;
    
    /** The property map to use to extract parameters */
    PropertyMap propmap;
    
    typedef QVector<detail::CLJ14AtomPair> CLJ14AtomData;
    
    /** The 14 pair data for all CutGroups, and between all 
        bonded pairs of CutGroups */
    QVector<CLJ14AtomData> data_for_pair;

    /** The indicies of all elements in 'cgroups' that contains 14 data
        about the CutGroup at CGIdx 'cgidx' */
    QHash< quint32, QSet<quint32> > cgidx_to_idx;
    
    /** The index of the element that contains 14 data between each
        pair of bonded cutgroups */
    QHash< detail::IDPair, qint32 > cgpair_to_idx;
    
    /** The current coulomb and LJ energies */
    double total_cnrg, total_ljnrg;
    
    /** Whether or not the energy needs to be recalculated */
    bool needs_energy;
};

}

Q_DECLARE_METATYPE( SireMM::CLJ14Group )

SIRE_EXPOSE_CLASS( SireMM::CLJ14Group )

SIRE_END_HEADER

#endif
