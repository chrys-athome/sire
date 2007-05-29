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

#ifndef SIRESYSTEM_MOLMAPPINGFUNCTION_H
#define SIRESYSTEM_MOLMAPPINGFUNCTION_H

#include "SireVol/mappingfunction.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

using SireVol::MappingFunctionBase;

/** This is the base class of functions that map molecules from one space
    to another.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MolMappingFunctionBase : public MappingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolMappingFunctionBase&);
friend QDataStream& ::operator>>(QDataStream&, MolMappingFunctionBase&);

public:
    MolMappingFunction(const MappingFunction &mappingfunction);
    
    MolMappingFunction(const MolMappingFunction &other);
    
    ~MolMappingFunction();
    
    CoordGroup operator()(const CoordGroup &coords,
                          const Space &old_space,
                          const Space &new_space) const;
    
    QVector<CoordGroup> operator()(const QVector<CoordGroup> &coords,
                                   const Space &old_space,
                                   const Space &new_space) const;
    
    PartialMolecule operator()(const PartialMolecule &molecule,
                               const Space &old_space,
                               const Space &new_space) const;
                               
    Molecules operator()(const Molecules &molecules,
                         const Space &old_space,
                         const Space &new_space) const;
    
    static const char* typeName()
    {
        return "SireSystem::MolMappingFunction";
    }

    QVector<CoordGroup> map(const QVector<CoordGroup> &coords,
                            const Space &old_space,
                            const Space &new_space) const;
                                       
    CoordGroup map(const CoordGroup &coords,
                   const Space &old_space,
                   const Space &new_space) const;
                   
    virtual PartialMolecule map(const PartialMolecule &molecule,
                                const Space &old_space,
                                const Space &new_space) const=0;
                                
    virtual Molecules map(const Molecules &molecules,
                          const Space &old_space,
                          const Space &new_space) const=0;
};

/** This function maps each molecule as a single unit - this
    means that even multi-cutgroup molecules are mapped as a 
    single unit.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MapAsWholeMolecule : public MolMappingFunctionBase
{
};

/** This function maps each cutgroup within each molecule as 
    separate units. This will change the internal geometry
    of multi-cutgroup molecules.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MapAsCutGroups : public MolMappingFunctionBase
{
};

}

Q_DECLARE_METATYPE(SireSystem::MapAsWholeMolecule);
Q_DECLARE_METATYPE(SireSystem::MapAsCutGroups);
Q_DECLARE_METATYPE(SireSystem::MolMappingFunction);

SIRE_END_HEADER

#endif
