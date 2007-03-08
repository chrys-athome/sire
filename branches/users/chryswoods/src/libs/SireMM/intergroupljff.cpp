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

#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "intergroupljff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireStream;

///////////
/////////// Implementation of InterGroupLJFF::Components
///////////

/** Constructor */
InterGroupLJFF::Components::Components() : LJFF::Components()
{}

/** Constructor that just passes its arguments up to the parent's constructor */
InterGroupLJFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
               : LJFF::Components(ffbase,symbols)
{}

/** Copy constructor */
InterGroupLJFF::Components::Components(const Components &other)
               : LJFF::Components(other)
{}

/** Destructor */
InterGroupLJFF::Components::~Components()
{}

///////////
/////////// Implementation of InterGroupLJFF::Parameters
///////////

/** Constructor */
InterGroupLJFF::Parameters::Parameters() : LJFF::Parameters()
{}

/** Copy constructor */
InterGroupLJFF::Parameters::Parameters(const Parameters &other)
               : LJFF::Parameters(other)
{}

/** Destructor */
InterGroupLJFF::Parameters::~Parameters()
{}

///////////
/////////// Implementation of InterGroupLJFF::Groups
///////////

/** Constructor */
InterGroupLJFF::Groups::Groups() : LJFF::Groups()
{}

/** Copy constructor */
InterGroupLJFF::Groups::Groups(const Groups &other)
               : LJFF::Groups(other)
{}

/** Destructor */
InterGroupLJFF::Groups::~Groups()
{}

///////////
/////////// Implementation of InterGroupLJFF
///////////

static const RegisterMetaType<InterGroupLJFF> r_interljff;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const InterGroupLJFF &interljff)
{
    writeHeader(ds, r_interljff, 1);
    
    SharedDataStream sds(ds);
    
    sds << interljff.mols[0]
        << interljff.mols[1]
        << interljff.changed_mols[0]
        << interljff.changed_mols[1]
        << interljff.removed_mols
        << static_cast<const LJFF&>(interljff);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, 
                                      InterGroupLJFF &interljff)
{
    VersionID v = readHeader(ds, r_interljff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> interljff.mols[0]
            >> interljff.mols[1]
            >> interljff.changed_mols[0]
            >> interljff.changed_mols[1]
            >> interljff.removed_mols
            >> static_cast<LJFF&>(interljff);
            
        //rebuild mols and changed_mols
        
        for (int group=0; group<2; ++group)
        {
            uint nmols = interljff.mols[group].count();
        
            if (nmols > 0)
            {
                QHash<MoleculeID,uint> molid_to_index;
                molid_to_index.reserve(nmols);
        
                const LJFF::LJMolecule *mol_array = interljff.mols[group].constData();
        
                for (uint i=0; i<nmols; ++i)
                {
                    molid_to_index.insert( mol_array[i].molecule().ID(), i );
                }
            
                interljff.molid_to_index[group] = molid_to_index;
            }
            else
                interljff.molid_to_index[group].clear();
        
            nmols = interljff.changed_mols[group].count();
        
            if (nmols > 0)
            {
                QHash<MoleculeID,uint> molid_to_changedindex;
                molid_to_changedindex.reserve(nmols);
              
                const LJFF::ChangedLJMolecule *mol_array 
                                = interljff.changed_mols[group].constData();
            
                for (uint i=0; i<nmols; ++i)
                {
                    MoleculeID molid = mol_array[i].oldMolecule().molecule().ID();
                
                    if (molid == 0)
                        molid = mol_array[i].newMolecule().molecule().ID();
                    
                    BOOST_ASSERT(molid != 0);
                    
                    molid_to_changedindex.insert( molid, i );
                }
            
                interljff.molid_to_changedindex[group] = molid_to_changedindex;
            }
            else
                interljff.molid_to_changedindex[group].clear();
        }
    }
    else
        throw version_error(v, "1", r_interljff, CODELOC);
    
    return ds;
}

/** Constructor */
InterGroupLJFF::InterGroupLJFF() : LJFF()
{}

/** Construct a LJ forcefield using the passed Space, combining rules and
    switching function */
InterGroupLJFF::InterGroupLJFF(const Space &space, 
                               const SwitchingFunction &switchfunc)
               : LJFF(space, switchfunc)
{}

/** Copy constructor */
InterGroupLJFF::InterGroupLJFF(const InterGroupLJFF &other)
               : LJFF(other), 
                 removed_mols(other.removed_mols)
{
    for (int i=0; i<2; ++i)
    {
        mols[i] = other.mols[i];
        molid_to_index[i] = other.molid_to_index[i];
        
        changed_mols[i] = other.changed_mols[i];
        molid_to_changedindex[i] = other.molid_to_changedindex[i];
    }
}

/** Destructor */
InterGroupLJFF::~InterGroupLJFF()
{}

/** Assignment operator */
InterGroupLJFF& InterGroupLJFF::operator=(const InterGroupLJFF &other)
{
    for (int i=0; i<2; ++i)
    {
        mols[i] = other.mols[i];
        molid_to_index[i] = other.molid_to_index[i];
        
        changed_mols[i] = other.changed_mols[i];
        molid_to_changedindex[i] = other.molid_to_changedindex[i];
    }
    
    removed_mols = other.removed_mols;
    
    return *this;
}

/** Recalculate the total energy of this forcefield from scratch */
void InterGroupLJFF::recalculateTotalEnergy()
{
    //calculate the total LJ energy between pairs of 
    //molecules between the two groups

    double ljnrg = 0;
    
    int nmols0 = mols[0].count();
    int nmols1 = mols[1].count();
    
    if (nmols0 > 0 and nmols1 > 0)
    {
        const LJMolecule *mols0_array = mols[0].constData();
        const LJMolecule *mols1_array = mols[1].constData();
        
        for (int i=0; i<nmols0; ++i)
        {
            const LJMolecule &mol0 = mols0_array[i];
            
            for (int j=0; j<nmols1; ++j)
            {
                const LJMolecule &mol1 = mols1_array[j];
                
                ljnrg += LJFF::calculateEnergy( mol0, mol1,
                                                space(), 
                                                switchingFunction(),
                                                distanceMatrix(),
                                                ljMatrix() );
            }
        }
    }

    this->setComponent( components().lj(), ljnrg );
    this->setComponent( components().total(), ljnrg );

    //clear the list of changed molecules
    changed_mols.clear();
    molid_to_changedindex.clear();
    removed_mols.clear();
}

/** Recalculate the energy by using a delta from the old configuration */
void InterGroupCLJFF::recalculateViaDelta()
{
    int nmols0 = mols[0].count();
    int nmols1 = mols[1].count();
    
    int nchanged0 = changed_mols[0].count();
    int nchanged1 = changed_mols[1].count();
    
    if ( (nchanged0 == 0 and nchanged1 == 0) or n_a_mols == 0 or n_b_mols == 0 )
    {
        //no changed molecules, or no molecules, 
        //so just recalculate the total energy
        this->recalculateTotalEnergy();
        return;
    }
    
    double ljnrg = 0;
    
    //calculate the change in energy of group 0 with group 1

}

