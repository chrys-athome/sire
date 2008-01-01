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

#ifndef SIREMOL_MOLGROUPS_H
#define SIREMOL_MOLGROUPS_H

namespace SireMol
{
class MolGroupsBase;
class MolGroups;
}

namespace SireMol
{

/** This is the base class of all MoleculeGroups objects.
    These are containers for MoleculeGroup objects, thereby
    allowing lots of MoleculeGroup objects to be collected
    together and indexed (e.g. so that you can find all
    CA atoms in the "proteins" group). This is the virtual
    base class of the hierarchy - see MolGroups for a simple
    concrete instantiation.
    
    @author Christopher Woods
*/
class MolGroupsBase : public PropertyBase
{

};

/** This class holds a collection of MoleculeGroup objects. This
    allows multiple groups to be themselves grouped together.
    This is a virtual class, which can hold the virtual
    MoleculeGroup class objects. This can be used usefully in
    several situations, e.g.;
    
    System is derived from MolGroupsBase, and uses the MolGroups
    code to manage the indexing and version management of all
    of the molecules in the system.
    
    The forcefields are also derived from MolGroupsBase, allowing
    the MolGroups code to do the indexing and version management
    of molecules in a forcefield. Also, this allows easy 
    management of multiple groups in a forcefield, e.g.
    QM molecules and MM molecules, or group A and group B.
    
    While System and the forcefields are derived from MolGroupsBase,
    this class, MolGroups, provides a concrete class that allows
    the user to easily group together different MoleculeGroups.
    
    @author Christopher Woods
*/
class MolGroups : public ConcreteProperty<MolGroups,MolGroupsBase>
{

friend QDataStream& ::operator<<(QDataStream&, const MolGroups&);
friend QDataStream& ::operator>>(QDataStream&, MolGroups&);

public:
    MolGroups();
    
    MolGroups(const MolGroups &other);
    
    ~MolGroups();
    
private:
    /** All of the MoleculeGroup objects in this collection, 
        indexed by molecule group number */
    QHash<MGNum,MoleculeGroup> mgroups;
};

}

Q_DECLARE_METATYPE(SireMol::MolGroups);

#endif
