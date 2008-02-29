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

#ifndef SIREFF_DETAIL_FFMOLECULES_H
#define SIREFF_DETAIL_FFMOLECULES_H

#include "SireMol/partialmolecule.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

namespace detail
{
class FFMoleculeBase;
class FFMoleculesBase;

template<class PTNL>
class FFMolecule;

template<class PTNL>
class FFMolecules;
}

}

QDataStream& operator<<(QDataStream&, const SireFF::detail::FFMoleculeBase&);
QDataStream& operator>>(QDataStream&, SireFF::detail::FFMoleculeBase&);

QDataStream& operator<<(QDataStream&, const SireFF::detail::FFMoleculesBase&);
QDataStream& operator>>(QDataStream&, SireFF::detail::FFMoleculesBase&);

template<class PTNL>
QDataStream& operator<<(QDataStream&, const SireFF::detail::FFMolecule<PTNL>&);
template<class PTNL>
QDataStream& operator>>(QDataStream&, SireFF::detail::FFMolecule<PTNL>&);

template<class PTNL>
QDataStream& operator<<(QDataStream&, const SireFF::detail::FFMolecules<PTNL>&);
template<class PTNL>
QDataStream& operator>>(QDataStream&, SireFF::detail::FFMolecules<PTNL>&);

namespace SireMol
{
class MolNum;
}

namespace SireFF
{

namespace detail
{

using SireMol::PartialMolecule;
using SireMol::MolNum;

/** This is the base class of most (all?) of the forcefield
    specialised molecules. Forcefields used specialised molecule
    classes as a way of storing important data in a way that
    is best for the forcefield itself. As these specialised
    classes are only used within a forcefield, they do not
    form part of the public API of Sire, and should not be
    used in any other code.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFMoleculeBase
{

friend QDataStream& ::operator<<(QDataStream&, const FFMoleculeBase&);
friend QDataStream& ::operator>>(QDataStream&, FFMoleculeBase&);

public:
    FFMoleculeBase();
    FFMoleculeBase(const PartialMolecule &molview);
    
    ~FFMoleculeBase();
    
    bool operator==(const FFMoleculeBase &other) const;
    bool operator!=(const FFMoleculeBase &other) const;
    
    const PartialMolecule& molecule() const;

    MolNum number() const;

    bool isEmpty() const;

protected:
    FFMoleculeBase& operator=(const FFMoleculeBase &other);

    bool change(const PartialMolecule &molecule);
    
private:
    /** Copy of the view that this object represents */
    PartialMolecule mol;
};

/** This is the forcefield specific part of the FFMolecule class
    that holds the forcefield parameters for this part of the molecule
    
    @author Christopher Woods
*/
template<class PTNL>
class FFMolecule : public FFMoleculeBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const FFMolecule<PTNL>&);
friend QDataStream& ::operator>><>(QDataStream&, FFMolecule<PTNL>&);

public:
    typedef typename PTNL::Parameters Parameters;
    typedef typename PTNL::ParameterNames ParameterNames;

    FFMolecule();
    
    FFMolecule(const PartialMolecule &molecule,
               PTNL &forcefield,
               const ParameterNames &parameters);

    FFMolecule(const FFMolecule<PTNL> &other);

    ~FFMolecule();

    FFMolecule<PTNL>& operator=(const FFMolecule<PTNL> &other);
    
    bool operator==(const FFMolecule<PTNL> &other) const;
    bool operator!=(const FFMolecule<PTNL> &other) const;

    const Parameters& parameters() const;
    
    bool change(const PartialMolecule &molecule,
                PTNL &forcefield,
                const ParameterNames &parameternames);
    
    bool change(const PartialMolecule &molecule,
                const ParameterNames &new_paramnames,
                PTNL &forcefield,
                const ParameterNames &old_paramnames);

    FFMolecule<PTNL> getDifferences(const FFMolecule<PTNL> &other) const;

private:
    /** The parameters for this molecule */
    Parameters params;
};

/** This is the base class of most (all?) of the 'Molecules'
    classes. These classes hold groups of molecules together
    in a way that allows very rapid iteration over them and 
    their forcefield parameters. By its nature, this is a 
    template class, so this provides the template independent
    parts.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFMoleculesBase
{

friend QDataStream& ::operator<<(QDataStream&, const FFMoleculesBase&);
friend QDataStream& ::operator>>(QDataStream&, FFMoleculesBase&);

public:
    ~FFMoleculesBase();

    const QVector<MolNum> molNumsByIndex() const;
    const QHash<MolNum,quint32>& indexesByMolNum() const;

    quint32 indexOf(MolNum molnum) const;
    bool contains(MolNum molnum) const;

    void assertContains(MolNum molnum) const;

protected:
    FFMoleculesBase();
    
    FFMoleculesBase(const FFMoleculesBase &other);
    
    FFMoleculesBase& operator=(const FFMoleculesBase &other);

    bool operator==(const FFMoleculesBase &other) const;
    bool operator!=(const FFMoleculesBase &other) const;

    quint32 _pvt_add(MolNum molnum);

    void _pvt_remove(MolNum molnum);
    void _pvt_remove(const QList<MolNum> &molnums);
    
private:
    void _pvt_reindex();

    /** The number of each molecule in the group, 
        in the order they appear in the arrays */
    QVector<MolNum> molnums_by_idx;
    
    /** The indicies of each molecule in the array
        indexed by molecule number */
    QHash<MolNum,quint32> idxs_by_molnum;
};

/** This class provides an array of FF specialised molecules, 
    arranged in such a way as to speed up indexing over them.
    
    The template parameter is the potential type for which 
    this group is used (e.g. SireMM::CLJPotential)
    
    This is necessary, as different
    potentials use different parameterisation methods,
    and can supply different specialised molecule types.
    
    @author Christopher Woods
*/
template<class PTNL>
class FFMolecules : public FFMoleculesBase
{
public:
    typedef typename PTNL::Molecule Molecule;
    typedef typename PTNL::ChangedMolecule ChangedMolecule;

    typedef typename PTNL::ParameterNames ParameterNames;

    FFMolecules();
    
    FFMolecules(const FFMolecules<PTNL> &other);
    
    ~FFMolecules();

    FFMolecules<PTNL>& operator=(const FFMolecules<PTNL> &other);
    
    bool operator==(const FFMolecules<PTNL> &other) const;
    bool operator!=(const FFMolecules<PTNL> &other) const;

    ChangedMolecule change(const PartialMolecule &molecule,
                           PTNL &forcefield);
                           
    ChangedMolecule add(const PartialMolecule &molecule,
                        PTNL &forcefield);
                        
    ChangedMolecule remove(const PartialMolecule &molecule,
                           PTNL &forcefield);
    
    const QVector<Molecule>& moleculesByIndex() const;
    const QVector<ParameterNames>& parameterNamesByIndex() const;
    
protected:
    /** The array of forcefield-specialised molecules in this group */
    QVector<Molecule> mols_by_idx;
    
    /** The current parameter locations used for each molecule */
    QVector<ParameterNames> parameter_names;
};

/** This implementation class holds the information about 
    a change in a molecule. This can be used by a
    forcefield to simplify the calculation of the change in
    energy associated with a change in the molecule(s)
    (e.g. if only changes in energy associated with the 
    changed molecules needs to be evaluated) 
    
    The template parameter 'FFMOL' is the specialised Molecule
    type used by the forcefield. 
    
    @author Christopher Woods
*/
template<class FFMOL>
class ChangedMolecule
{
public:
    typedef FFMOL Molecule;

    ChangedMolecule();
    
    ChangedMolecule(const FFMOL &molecule);
    ChangedMolecule(const FFMOL &oldmol, const FFMOL &newmol);
    
    ChangedMolecule(const ChangedMolecule<FFMOL> &other);
    
    ~ChangedMolecule();
    
    ChangedMolecule<FFMOL>& operator=(const ChangedMolecule<FFMOL> &other);
    
    bool operator==(const ChangedMolecule<FFMOL> &other) const;
    bool operator!=(const ChangedMolecule<FFMOL> &other) const;
    
    MolNum number() const;
    
    bool isEmpty() const;
    
    bool changedAll() const;
    bool nothingChanged() const;
    
    const FFMOL& oldMolecule() const;
    const FFMOL& newMolecule() const;
    
    const FFMOL& oldParts() const;
    const FFMOL& newParts() const;
    
    ChangedMolecule<FFMOL>& change(const FFMOL &new_molecule);
    
private:
    /** The complete, old molecule */
    FFMOL old_molecule;
    
    /** The complete, new molecule */
    FFMOL new_molecule;
    
    /** The parts of the old molecule that have changed */
    FFMOL old_parts;
    
    /** The corresponding changed parts of the new molecule */
    FFMOL new_parts;
};

////////
//////// Implementation of FFMolecule
////////

/** Null constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule<PTNL>::FFMolecule() : FFMoleculeBase()
{}

/** Create a specialised version of the molecule 'molecule'
    that will be part of the passed forcefield 'forcefield',
    using the parameter names in 'parameternames' to find
    and extract the correct parameters */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule<PTNL>::FFMolecule(const PartialMolecule &molecule,
                               PTNL &forcefield,
                               const ParameterNames &parameternames)
                   : FFMoleculeBase(),
                     params( forcefield.parameterise(molecule,
                                                     parameternames) )
{}

/** Copy constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule<PTNL>::FFMolecule(const FFMolecule<PTNL> &other)
                   : FFMoleculeBase(other), params(other.params)
{}

/** Destructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule<PTNL>::~FFMolecule()
{}

/** Copy assignment operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule<PTNL>& FFMolecule<PTNL>::operator=(const FFMolecule<PTNL> &other)
{
    FFMoleculeBase::operator=(other);
    params = other.params;
    
    return *this;
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule<PTNL>::operator==(const FFMolecule<PTNL> &other) const
{
    return FFMoleculeBase::operator==(other) and 
           params == other.params;
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule<PTNL>::operator!=(const FFMolecule<PTNL> &other) const
{
    return FFMoleculeBase::operator!=(other) or
           params != other.params;
}

/** Return the parameters for this view of the molecule. These parameters
    are specialised for the particular forcefield that this object
    was parameterised in (so will not necessarily make sense outside
    that forcefield) */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
const typename FFMolecule<PTNL>::Parameters& FFMolecule<PTNL>::parameters() const
{
    return params;
}

/** Change this molecule so that it is equal to 'new_molecule'.

    The names of the properties used to originally get the 
    parameters for this molecule are in 'parameternames'
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule<PTNL>::change(const PartialMolecule &new_molecule,
                                PTNL &forcefield,
                    const typename FFMolecule<PTNL>::ParameterNames &parameternames)
{
    //save the old molecule in case it needs to be restored
    PartialMolecule old_molecule = FFMoleculeBase::molecule();
    
    //change the molecule itself...
    bool mol_changed = FFMoleculeBase::change(new_molecule);
    
    if (mol_changed)
    {
        try
        {
            //now try to change the parameters
            params = forcefield.updateParameters(params, new_molecule,
                                                 old_molecule, parameternames);
        }
        catch(...)
        {
            //something went wrong... restore the old molecule
            FFMoleculeBase::change(old_molecule);
            throw;
        }
        
        return true;
    }
}

/** Change this molecule so that it is equal to 'new_molecule' and
    so that it gets its parameters from 'new_parameternames'

    The names of the properties used to originally get the 
    parameters for this molecule are in 'parameternames'
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule<PTNL>::change(const PartialMolecule &new_molecule,
                  const typename FFMolecule<PTNL>::ParameterNames &new_parameternames,
                                PTNL &forcefield,
                  const typename FFMolecule<PTNL>::ParameterNames &parameternames)
{
    if (new_parameternames == parameternames)
        //we are not changing the parameter sources
        return this->change(new_molecule, forcefield, parameternames);

    //save the old version of the molecule
    PartialMolecule old_molecule = FFMoleculeBase::molecule();
    
    bool changed = FFMoleculeBase::change(new_molecule);
    
    try
    {
        Parameters new_params = forcefield.paramterise(new_molecule,
                                                       new_parameternames);

        changed = changed or (new_params != params);
        
        params = new_params;
    }
    catch(...)
    {
        //restore the old molecule
        FFMoleculeBase::change(old_molecule);
        throw;
    }
    
    return changed;
}

///////
/////// Implementation of FFMolecules
///////


///////
/////// Implementation of ChangedMolecule
///////

/** Null constructor */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>::ChangedMolecule()
{}

/** Construct from just the old molecule. This represents no change */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>::ChangedMolecule(const FFMOL &molecule)
                       : old_molecule(molecule),
                         new_molecule(molecule)
{}

/** Construct the change from 'oldmol' to 'newmol'. Both 'oldmol'
    and 'newmol' must be views of the same molecule (i.e. their
    molecule numbers must be identical!)

    \throw SireError::incompatible_error
*/
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>::ChangedMolecule(const FFMOL &oldmol, const FFMOL &newmol)
                       : old_molecule(oldmol),
                         new_molecule(newmol)
{
    if (oldmol.isEmpty())
    {
        //this represents creating newmol
        new_parts = new_molecule;
    }
    else if (newmol.isEmpty())
    {
        //this represent destroying oldmol
        old_parts = old_molecule;
    }
    else
    {
        //we are changing from oldmol to newmol
        SireFF::detail::assertCompatible(old_molecule, new_molecule);
        
        //now work out the differences
        if (oldmol != newmol)
        {
            old_parts = old_molecule.getDifferences(new_molecule);
            new_parts = new_molecule.getDifferences(old_molecule);
        }
    }
}

/** Copy constructor */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>::ChangedMolecule(const ChangedMolecule<FFMOL> &other)
                       : old_molecule(other.old_molecule),
                         new_molecule(other.new_molecule),
                         old_parts(other.old_parts),
                         new_parts(other.new_parts)
{}

/** Destructor */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>::~ChangedMolecule()
{}

/** Copy assignment operator */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>& ChangedMolecule<FFMOL>::operator=(
                                            const ChangedMolecule<FFMOL> &other)
{
    if (this != &other)
    {
        old_molecule = other.old_molecule;
        new_molecule = other.new_molecule;
        old_parts = other.old_parts;
        new_parts = other.new_parts;
    }
    
    return *this;
}

/** Comparison operator */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
bool ChangedMolecule<FFMOL>::operator==(const ChangedMolecule<FFMOL> &other) const
{
    return this == &other or
           (old_molecule == other.old_molecule and 
            new_molecule == other.new_molecule and
            old_parts == other.old_parts and
            new_parts == other.new_parts);
}

/** Comparison operator */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
bool ChangedMolecule<FFMOL>::operator!=(const ChangedMolecule<FFMOL> &other) const
{
    return not this->operator==(other);
}

/** Return the number of this molecule */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
MolNum ChangedMolecule<FFMOL>::number() const
{
    if (old_molecule.isEmpty())
        return new_molecule.number();
    else
        return old_molecule.number();
}

/** Return whether this is an empty change (contains nothing!) */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
bool ChangedMolecule<FFMOL>::isEmpty() const
{
    return old_molecule.isEmpty() and new_molecule.isEmpty();
}

/** Return whether or not the entire molecule has changed */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
bool ChangedMolecule<FFMOL>::changedAll() const
{
    return new_molecule == new_parts and 
           old_molecule == old_parts;
}

/** Return whether nothing has changed */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
bool ChangedMolecule<FFMOL>::nothingChanged() const
{
    return new_molecule == old_molecule;
}

/** Return the complete old version of the molecule
    (this may be empty if this represents the creation of a molecule) */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
const FFMOL& ChangedMolecule<FFMOL>::oldMolecule() const
{
    return old_molecule;
}

/** Return the complete new version of the molecule
    (this may be empty if this represents the removal of a molecule) */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
const FFMOL& ChangedMolecule<FFMOL>::newMolecule() const
{
    return new_molecule;
}

/** Return the parts of the old molecule that have changed */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
const FFMOL& ChangedMolecule<FFMOL>::oldParts() const
{
    return old_parts;
}

/** Return the parts of the new molecule that have changed */
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
const FFMOL& ChangedMolecule<FFMOL>::newParts() const
{
    return new_parts;
}

/** Further change this change so that it equals the change
    from old_molecule to new_new_molecule
    
    \throw SireError::incompatible_error
*/
template<class FFMOL>
SIRE_OUTOFLINE_TEMPLATE
ChangedMolecule<FFMOL>& ChangedMolecule<FFMOL>::change(
                                            const FFMOL &new_new_molecule)
{
    if (new_new_molecule.isEmpty())
    {
        //we are now removing the molecule
        new_molecule = new_new_molecule;
        new_parts = FFMOL();
        old_parts = old_molecule;
    }
    else if (old_molecule.isEmpty())
    {
        //we are changing an added molecule - ensure that it is compatible
        if (not new_molecule.isEmpty())
            SireFF::detail::assertCompatible(new_molecule, new_new_molecule);

        new_molecule = new_new_molecule;
        new_parts = new_new_molecule;
    }
    else
    {
        SireFF::detail::assertCompatible(old_molecule, new_molecule);
        
        new_molecule = new_new_molecule;
        
        if (new_molecule != old_molecule)
        {
            old_parts = old_molecule.getDifferences(new_molecule);
            new_parts = new_molecule.getDifferences(old_molecule);
        }
        else
        {
            //this represents no change
            new_molecule = old_molecule;
            
            old_parts = FFMOL();
            new_parts = FFMOL();
        }
    }
    
    return *this;
}

}

}

SIRE_END_HEADER

#endif
