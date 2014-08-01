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

#ifndef SIREMM_CLJGROUP_H
#define SIREMM_CLJGROUP_H

#include "cljextractor.h"

#include "SireMol/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJGroup;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJGroup&);
QDataStream& operator>>(QDataStream&, SireMM::CLJGroup&);

namespace SireMM
{

using SireMol::MolNum;
using SireMol::MoleculeGroup;

/** This class holds and manages a group of molecules that have been 
    added to a CLJBoxes object. 
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJGroup
{

friend QDataStream& ::operator<<(QDataStream&, const SireMM::CLJGroup&);
friend QDataStream& ::operator>>(QDataStream&, SireMM::CLJGroup&);

public:
    CLJGroup();
    CLJGroup(const QString &name);
    CLJGroup(const QString &name, const MoleculeView &molview);
    CLJGroup(const QString &name, const Molecules &molecules);
    CLJGroup(const QString &name, const MoleculeGroup &molgroup);

    CLJGroup(const CLJGroup &other);
    
    ~CLJGroup();
    
    MoleculeGroup group() const;
    
    void add(const MoleculeView &molview);
    void add(const Molecules &molecules);
    
    void update(const MoleculeView &molview);
    void update(const Molecules &molecules);
    
    void updatedConnectedGroup();
    
    void remove(const MoleculeView &molview);
    void remove(const Molecules &molecules);
    
    void remove(MolNum molnum);
    
    void removeAll();
    
    void accept();
    
    const CLJBoxes& cljBoxes() const;
    CLJAtoms changedAtoms() const;
    
    void mustRecalculateFromScratch();
    
    void mustReallyRecalculateFromScratch();
    
private:
    /** Molecule group containing all of the molecules in this group */
    MoleculeGroup molgroup;

    /** All of the extractors that manage extracting the charge and LJ 
        properties from all of the molecules */
    QHash<MolNum,CLJExtractor> cljexts;
    
    /** The boxes to which the CLJAtoms from each molecule in this group
        have been added */
    CLJBoxes cljboxes;
    
    /** The workspace used to cache information about moves */
    CLJWorkspace cljworkspace;
    
    /** All of the changed molecules */
    QSet<MolNum> changed_mols;
};

}

Q_DECLARE_METATYPE( SireMM::CLJGroup )

SIRE_EXPOSE_CLASS( SireMM::CLJGroup )

SIRE_END_HEADER

#endif
