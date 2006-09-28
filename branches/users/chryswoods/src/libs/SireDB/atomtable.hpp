#ifndef SIREDB_ATOMTABLE_HPP
#define SIREDB_ATOMTABLE_HPP

#include "atomtable.h"

#include <QDebug>

#include "SireError/errors.h"
#include "SireDB/errors.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class T>
class AtomTableT;
}

template<class T>
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<T>&);
template<class T>
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<T>&);

namespace SireDB
{

/** This template class provides the additional functionality for
    an AtomTable that is used to store parameters of type 'T'.

    @author Christopher Woods
*/
template<class Param>
class SIREDB_EXPORT AtomTableT : public AtomTable
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomTableT<Param>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomTableT<Param>&);

public:
    typedef Param parameter_type;

    AtomTableT();
    AtomTableT(const MoleculeInfo &molinfo);

    AtomTableT(const AtomTableT<Param> &other);

    ~AtomTableT();

    const Param& at(const CGAtomID &cgatomid) const;
    const Param& at(const ResNumAtomID &resatomid) const;
    const Param& at(const ResIDAtomID &resatomid) const;
    const Param& at(const AtomIndex &atom) const;
    const Param& at(AtomID atomid) const;

    const Param& operator[](const CGAtomID &cgatomid) const;
    const Param& operator[](const ResNumAtomID &resatomid) const;
    const Param& operator[](const ResIDAtomID &resatomid) const;
    const Param& operator[](const AtomIndex &atom) const;
    const Param& operator[](AtomID atomid) const;

    Param& operator[](const CGAtomID &cgatomid);
    Param& operator[](const ResNumAtomID &resatomid);
    Param& operator[](const ResIDAtomID &resatomid);
    Param& operator[](const AtomIndex &atom);
    Param& operator[](AtomID atomid);

    Param value(const CGAtomID &cgatomid) const;
    Param value(const ResNumAtomID &resatomid) const;
    Param value(const ResIDAtomID &resatomid) const;
    Param value(const AtomIndex &atom) const;
    Param value(AtomID atomid) const;

    Param value(const CGAtomID &cgatomid, const Param &defaultValue) const;
    Param value(const ResNumAtomID &resatomid, const Param &defaultValue) const;
    Param value(const ResIDAtomID &resatomid, const Param &defaultValue) const;
    Param value(const AtomIndex &atom, const Param &defaultValue) const;
    Param value(AtomID atomid, const Param &defaultValue) const;

    const Param& getParameter(const CGAtomID &cgatomid) const;
    const Param& getParameter(const ResNumAtomID &resatomid) const;
    const Param& getParameter(const ResIDAtomID &resatomid) const;
    const Param& getParameter(const AtomIndex &atom) const;
    const Param& getParameter(AtomID atomid) const;

    void setParameter(const CGAtomID &cgatomid, const Param &param);
    void setParameter(const ResNumAtomID &resatomid, const Param &param);
    void setParameter(const ResIDAtomID &resatomid, const Param &param);
    void setParameter(const AtomIndex &atom, const Param &param);
    void setParameter(AtomID atomid, const Param &param);

    void add(const TableBase &table);

    QVector<Param> parameters() const;
    QVector<Param> parameters(ResNum resnum) const;
    QVector<Param> parameters(ResID resid) const;
    QVector<Param> parameters(CutGroupID cgid) const;

    ParameterGroup<Param> parameterGroup(CutGroupID cgid) const;

    QHash< CutGroupID,ParameterGroup<Param> > parameterGroups() const;
    QHash< CutGroupID,ParameterGroup<Param> >
        parameterGroups( const QSet<CutGroupID> &cgids ) const;

    QHash< CutGroupID,QVector<Param> >
          parameters(const QSet<CutGroupID> &cgids) const;
    QHash< ResNum,QVector<Param> >
          parameters(const QSet<ResNum> &resnums) const;
    QHash< ResID,QVector<Param> >
          parameters(const QSet<ResID> &resids) const;

    template<class C>
    void parameters(CutGroupID cgid, QVector<C> &params) const;
    template<class C>
    void parameters(ResNum resnum, QVector<C> &params) const;
    template<class C>
    void parameters(ResID resid, QVector<C> &params) const;

protected:
    int _unsafe_nAssignedParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nAssignedParameters(CutGroupID cgid) const;

    int _unsafe_nMissingParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nMissingParameters(CutGroupID cgid) const;

    bool _unsafe_assignedParameter(const CGAtomID &cgatomid) const;

    bool _unsafe_hasMissingParameters(const ResidueInfo &resinfo) const;
    bool _unsafe_hasMissingParameters(CutGroupID cgid) const;

    QSet<AtomIndex> _unsafe_missingParameters(const ResidueInfo &resinfo) const;
    QSet<AtomIndex> _unsafe_missingParameters(CutGroupID cgid) const;

    QVector<Param> _unsafe_parameters(const ResidueInfo &resinfo) const;
    QVector<Param> _unsafe_parameters(CutGroupID cgid) const;

    ParameterGroup<Param> _unsafe_parameterGroup(CutGroupID cgid) const;

    void _unsafe_clear(const ResidueInfo &resinfo);
    void _unsafe_clear(CutGroupID cgid);
    void _unsafe_clear(const CGAtomID &cgatomid);


private:

    /** The parameters in the table, indexed
        by CGAtomID */
    ParameterStore<CutGroupID,AtomID,Param> params;
};

/** Empty constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomTableT<Param>::AtomTableT() : AtomTable()
{}

/** Construct a table to hold the parameters for 'molecule' */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomTableT<Param>::AtomTableT(const Molecule &molecule) : AtomTable(molecule)
{
    //create space for the parameters for each atom in the molecule
    params = ParameterStore<CGAtomID,Param>( this->nAtomsPerCutGroup() );
}

/** Copy constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomTableT<Param>::AtomTableT(const AtomTableT<Param> &other)
             : AtomTable(other), params(other.params)
{}

/** Destructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomTableT<Param>::~AtomTableT()
{}

/** Return the number of assigned parameters - this will equal nAtoms()
    when all of the parameters have been assigned. */
template<class Param>
SIRE_INLINE_TEMPLATE
int AtomTableT<Param>::nParameters() const
{
    return params.nParameters();
}

/** Return the number of assigned parameters in the residue 'resnum'. This
    will equal nAtoms(resnum) when all of the parameters in this residue
    have been assigned.

    This throws an exception if this residue is not in the molecule.

    \throw SireMol::missing_residue
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
int AtomTableT<Param>::nParameters(ResNum resnum) const
{
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    int nats = resinfo.nAtoms();
    int nparams = 0;

    //count up the number of assigned parameters in this residue
    for (int i=0; i<nats; ++i)
    {
        if (params.assignedParameter( resinfo[i] ))
            nparams++;
    }

    return nparams;
}

/** Return the number of assigned parameters in the CutGroup with ID 'cgid'.
    This will be equal to nAtoms(cgid) if all of the parameters in this
    CutGroup have been assigned.

    This throws an exception if this CutGroup is not in the molecule.ID()

    \throw SireMol::missing_cutgroup
*/
template<class Param>
SIRE_INLINE_TEMPLATE
int AtomTableT<Param>::nParameters(CutGroupID cgid) const
{
    return params.nParameters(cgid);
}

/** Return the parameter at index 'cgid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::at(const CGAtomID &cgid) const
{
    return params.at(cgid);
}

/** Return the parameter at index 'rsid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::at(const ResNumAtomID &rsid) const
{
    return params.at( this->info()[rsid] );
}

/** Return the parameter of the atom 'atom' - this will
    throw an exception if this atom is not in the molecule,
    or if its parameter has yet to be assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::at(const AtomIndex &atom) const
{
    return params.at( this->info()[atom] );
}

/** Return the parameter at index 'cgid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::operator[](const CGAtomID &cgid) const
{
    return this->at(cgid);
}

/** Return the parameter at index 'rsid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::operator[](const ResNumAtomID &rsid) const
{
    return this->at(rsid);
}

/** Return the parameter of the atom 'atom' - this will
    throw an exception if this atom is not in the molecule,
    or if its parameter has yet to be assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::operator[](const AtomIndex &atom) const
{
    return this->at(atom);
}

/** Return a modifiable reference to the parameter for the atom
    with index 'cgid'. This will create a default-constructed
    parameter if it has yet to be assigned. This will throw
    an exception if this atom does not exist in the molecule.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param& AtomTableT<Param>::operator[](const CGAtomID &cgid)
{
    return params[cgid];
}

/** Return a modifiable reference to the parameter for the atom
    with index 'rsid'. This will create a default-constructed
    parameter if it has yet to be assigned. This will throw
    an exception if this atom does not exist in the molecule.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param& AtomTableT<Param>::operator[](const ResNumAtomID &rsid)
{
    return params[ this->info()[rsid] ];
}

/** Return a modifiable reference to the parameter for the atom
    'atom'. This will create a default-constructed
    parameter if it has yet to be assigned. This will throw
    an exception if this atom does not exist in the molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param& AtomTableT<Param>::operator[](const AtomIndex &atom)
{
    return params[ this->info()[atom] ];
}

/** Return the value of the parameter for the atom at index 'cgid'.
    This will return a default-constructed parameter if it has
    yet to be assigned. This will throw an exception if this atom
    is not in the molecule.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const CGAtomID &cgid) const
{
    return params.value(cgid);
}

/** Return the value of the parameter for the atom at index 'rsid'.
    This will return a default-constructed parameter if it has
    yet to be assigned. This will throw an exception if this atom
    is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const ResNumAtomID &rsid) const
{
    return params.value( this->info()[rsid] );
}

/** Return the value of the parameter for the atom 'atom'.
    This will return a default-constructed parameter if it has
    yet to be assigned. This will throw an exception if this atom
    is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const AtomIndex &atom) const
{
    return params.value( this->info()[atom] );
}

/** Return the value of the parameter for the atom at index 'cgid'.
    This will return a 'defaultValue' if it has yet to be assigned.
    This will throw an exception if this atom is not in the molecule.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const CGAtomID &cgid, const Param &defaultValue) const
{
    return params.value( cgid, defaultValue );
}

/** Return the value of the parameter for the atom at index 'rsid'.
    This will return a 'defaultValue' if it has yet to be assigned.
    This will throw an exception if this atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const ResNumAtomID &rsid, const Param &defaultValue) const
{
    return params.value( this->info()[rsid], defaultValue );
}

/** Return the value of the parameter for the atom 'atom'.
    This will return a 'defaultValue' if it has yet to be assigned.
    This will throw an exception if this atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
Param AtomTableT<Param>::value(const AtomIndex &atom, const Param &defaultValue) const
{
    return params.value( this->info()[atom], defaultValue );
}

/** Return the parameter at index 'cgid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::getParameter(const CGAtomID &cgid) const
{
    return this->at(cgid);
}

/** Return the parameter at index 'rsid' - this will throw
    an exception if this is an invalid index, or if
    the parameter has not yet been assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::getParameter(const ResNumAtomID &rsid) const
{
    return this->at(rsid);
}

/** Return the parameter of the atom 'atom' - this will
    throw an exception if this atom is not in the molecule,
    or if its parameter has yet to be assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AtomTableT<Param>::getParameter(const AtomIndex &atom) const
{
    return this->at(atom);
}

/** Set the value of the parameter for the atom with index 'cgid'
    to 'param'. This will overwrite any existing value of the
    this atom's parameter. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::setParameter(const CGAtomID &cgid, const Param &param)
{
    params.setParameter( cgid, param );
}

/** Set the value of the parameter for the atom with index 'rsid'
    to 'param'. This will overwrite any existing value of the
    this atom's parameter. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::setParameter(const ResNumAtomID &rsid, const Param &param)
{
    params.setParameter( this->info()[rsid], param );
}

/** Set the value of the parameter for the atom 'atom'
    to 'param'. This will overwrite any existing value of the
    this atom's parameter. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::setParameter(const AtomIndex &atom, const Param &param)
{
    params.setParameter( this->info()[atom], param );
}

/** Return whether or not the parameter for atom with index 'cgid'
    has been assigned. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
bool AtomTableT<Param>::assignedParameter(const CGAtomID &cgid) const
{
    return params.assignedParameter(cgid);
}

/** Return whether or not the parameter for atom with index 'rsid'
    has been assigned. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
bool AtomTableT<Param>::assignedParameter(const ResNumAtomID &rsid) const
{
    return params.assignedParameter( this->info()[rsid] );
}

/** Return whether or not the parameter for atom 'atom'
    has been assigned. This will throw an exception if this
    atom is not in the molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
bool AtomTableT<Param>::assignedParameter(const AtomIndex &atom) const
{
    return params.assignedParameter( this->info()[atom] );
}

/** Return whether or not there are any missing parameters */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AtomTableT<Param>::hasMissingParameters() const
{
    return params.hasMissingParameters();
}

/** Return whether or not there are any missing parameters in
    residue 'resnum'. This will throw an exception if this
    residue is not in the molecule.

    \throw SireMol::missing_residue
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool AtomTableT<Param>::hasMissingParameters(ResNum resnum) const
{
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    //loop over each atom in the residue and check that
    //its parameter has been assigned
    int nats = resinfo.nAtoms();
    for (int i=0; i<nats; ++i)
    {
        if (not params.assignedParameter(resinfo[i]))
            return false;
    }

    return true;
}

/** Return whether or not there are any missing parameters in
    the CutGroup with ID 'cgid'. This will throw an exception
    if this CutGroup is not in this molecule.

    \throw SireMol::missing_cutgroup
*/
template<class Param>
SIRE_INLINE_TEMPLATE
bool AtomTableT<Param>::hasMissingParameters(CutGroupID cgid) const
{
    return params.hasMissingParameters(cgid);
}

/** Return the set of all atoms that are missing parameters
    in the molecule. This will return an empty set if there are
    no missing parameters. */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<AtomIndex> AtomTableT<Param>::missingParameters() const
{
    return this->atoms( params.missingParameters() );
}

/** Return the set of all atoms that are missing parameters
    in residue 'resnum'. This will return an empty set if there are
    no missing parameters.

    This will throw an exception if this residue is not in the
    molecule.

    \throw SireMol::missing_residue
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<AtomIndex> AtomTableT<Param>::missingParameters(ResNum resnum) const
{
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    int nats = resinfo.nAtoms();
    QSet<AtomIndex> atms;

    //loop over all atoms in the residue and find those with
    //missing parameters
    for (int i=0; i<nats; ++i)
    {
        if ( not params.assignedParameter( resinfo[i] ) )
            atms.insert(resinfo.atom(i));
    }

    return atms;
}

/** Return the set of atoms with missing parameters in the CutGroup
    with ID 'cgid'. This will return an empty set if there are
    no missing parameters.

    This will throw an exception if this cutgroup is not in the
    molecule.

    \throw SireMol::missing_cutgroup
*/
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<AtomIndex> AtomTableT<Param>::missingParameters(CutGroupID cgid) const
{
    return this->atoms( params.missingParameters(cgid) );
}

/** Add the parameters from 'table' to this table. Note that 'table' must
    be of the type AtomTableT<Param> and that it must be compatible with
    this table or else an exception will be thrown

    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::add(const TableBase &table)
{
    const AtomTableT<Param> *param_table = dynamic_cast<const AtomTableT<Param>*>(&table);

    //is the table of the correct type?
    if (not param_table)
        throw SireError::invalid_cast(QObject::tr(
                "Could not cast the table of type \"%1\" to the required type \"%2\"")
                    .arg(table.what()).arg(this->what()), CODELOC);

    //are the tables compatible?
    if (this->info() != param_table->info())
        throw SireError::incompatible_error(QObject::tr(
                "Could not add the parameters from a %1 as it is incompatible with "
                "this table. (this->info() != table.info())")
                    .arg(this->what()), CODELOC);

    //is this table empty?
    if (params.nParameters() == 0)
    {
        //straight copy the parameters from the other table to this table
        params = param_table->params;
    }
    else
    {
        //ok - the table is of the correct type and is compatible. Loop
        //through every parameter in the table and add it to this table
        const QSet<CutGroupID> &cgroups = param_table->cutGroupIDs();

        for (QSet<CutGroupID>::const_iterator it = cgroups.begin();
             it != cgroups.end();
             ++it)
        {
            int natoms = param_table->nAtoms( *it );

            for (int i=0; i<natoms; ++i)
            {
                CGAtomID cgatmid( *it, i );

                if (param_table->assignedParameter(cgatmid))
                    this->setParameter( cgatmid, param_table->getParameter(cgatmid) );
            }
        }
    }
}

/** Clear all of the parameters from this table - note that this doesn't
    clear the atoms - it just clears the parameters. */
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::clear()
{
    params.clear();
}

/** Clear the parameter for the atom with index 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::clear(const CGAtomID &cgid)
{
    params.clear(cgid);
}

/** Clear the parameter for the atom with index 'cgid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::clear(const ResNumAtomID &rsid)
{
    params.clear( this->info()[rsid] );
}

/** Clear the parameter for the atom 'atom'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::clear(const AtomIndex &atom)
{
    params.clear( this->info()[atom] );
}

/** Clear all of the parameters for the atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::clear(ResNum resnum)
{
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    int nats = resinfo.nAtoms();
    for (int i=0; i<nats; ++i)
        params.clear( resinfo[i] );
}

/** Clear all of the parameters for the atoms in the CutGroup with
    ID == cgid.

    \throw SireMol::missing_cutgroup
*/
template<class Param>
SIRE_INLINE_TEMPLATE
void AtomTableT<Param>::clear(CutGroupID cgid)
{
    params.clear(cgid);
}

/** Return a vector of all of the parameters in the table. The parameters
    will be returned in the same order as the corresponding atoms that
    are returned by AtomTable::atoms()

    This will throw an exception if any of the parameters in the molecule
    have not yet been assigned.

    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param> AtomTableT<Param>::parameters() const
{
    QVector<Param> allparams;
    allparams.reserve( this->nAtoms() );

    //append all of the parameters for each CutGroup in turn
    QVector<CutGroupID> cgroups = this->orderedCutGroupIDs();
    const CutGroupID *array = cgroups.constData();

    int ncg = cgroups.count();
    for (int i=0; i<ncg; ++i)
    {
        allparams += params.parameters( array[i].index() );
    }

    return allparams;
}

/** Return a vector of all of the parameters in the residue with
    number 'resnum'. This will throw an exception if either the
    residue is not in this molecule, or if any of the parameters
    in the residue have not been assigned.

    \throw SireMol::missing_residue
    \throw SireDB::mising_parameter
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param> AtomTableT<Param>::parameters(ResNum resnum) const
{
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    int nats = resinfo.nAtoms();

    QVector<Param> allparams;
    allparams.reserve(nats);

    for (int i=0; i<nats; ++i)
    {
        allparams.append( params[ resinfo[i] ] );
    }

    return allparams;
}

/** Return a vector of all of the parameters in the CutGroup with
    ID 'cgid'. This will throw an exception if this CutGroup is
    not in this molecule, or if any of the atoms in the CutGroup
    are missing parameters.

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AtomTableT<Param>::parameters(CutGroupID cgid) const
{
    return params.parameters(cgid);
}

/** Return a GroupedVector of all of the parameters in the residues whose
    residue numbers are in 'resnums', indexed by ResNumAtomID. The indexes are
    the same as those contained in the metadata object returned by
    AtomTable::info(), and the parameters are in the same order as the
    corresponding atoms returned by AtomTable::atomsByResidue(resnums).

    This will throw an exception if any of the atoms are missing
    parameters, or if any of the residues are not in the molecule.

    \throw SireDB::missing_parameter
    \throw SireMol::missing_residue
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumAtomID,Param>
AtomTableT<Param>::parametersByResidue(const QSet<ResNum> &resnums) const
{
    int nres = resnums.count();

    GroupedVector<ResNumAtomID,Param> allparams;
    allparams.reserve(nres);

    for (QSet<ResNum>::const_iterator it = resnums.constBegin();
         it != resnums.constEnd();
         ++it)
    {
        allparams.insert( *it, this->parameters(*it) );
    }

    return allparams;
}

/** Return all of the parameters in the CutGroups whose ID numbers
    are in 'cgids', sorted into a GroupedVector indexed by CGAtomID.
    This will use the same indexing as the metadata object returned
    by AtomTable::info(), and returns the same order as the atoms
    returned by AtomTable::atomsByCutGroup(cgids)

    This will throw an exception if any of the atoms are missing
    parameters, or if any of the CutGroups are not in this molecule.

    \throw SireDB::missing_parameter
    \throw SireMol::missing_cutgroup
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<CGAtomID,Param>
AtomTableT<Param>::parametersByCutGroup(const QSet<CutGroupID> &cgids) const
{
    GroupedVector<CGAtomID,Param> allparams;
    allparams.reserve(cgids.count());

    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        allparams.insert( *it, params.parameters(*it) );
    }

    return allparams;
}

/** Return all of the parameters in this molecule sorted into a
    GroupedVector indexed by CGAtomID. This will use the same
    indexing as the metadata object returned by AtomTable::info(),
    and returns the same order as the atoms returned by
    AtomTable::atomsByCutGroup()

    This will throw an exception if any of the atoms are missing
    parameters.

    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<CGAtomID,Param> AtomTableT<Param>::parametersByCutGroup() const
{
    QVector<CutGroupID> cgroups = this->orderedCutGroupIDs();
    int ngroups = cgroups.count();
    const CutGroupID *array = cgroups.constData();

    GroupedVector<CGAtomID,Param> allparams;
    allparams.reserve(ngroups);

    for (int i=0; i<ngroups; ++i)
        allparams.insert( array[i], params.parameters(array[i]) );

    return allparams;
}

/** Return a GroupedVector of all of the parameters in the molecule indexed
    by ResNumAtomID. The indexes are the same as those contained in the
    metadata object returned by AtomTable::info(), and the parameters are
    in the same order as the corresponding atoms returned by
    AtomTable::atomsByResidue().

    This will throw an exception if any of the atoms are missing
    parameters.

    \throw SireDB::missing_parameter
*/
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumAtomID,Param> AtomTableT<Param>::parametersByResidue() const
{
    return parametersByResidue( this->residueNumbers() );
}

/** Copy all of the parameters intot the big vector 'param_vector', starting at
    index 'strt'. This works identically to 'parameters()' except that it copies
    the parameters into an existing vector. This vector must either be sufficiently
    large to hold the parameters (e.g. size() >= nAtoms() + strt) or it must be
    empty, in which case it will be automatically dimensioned to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    This returns the index that is one past the index of the last parameter
    that is copied into the array

    \throw SireError::invalid_arg
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
uint AtomTableT<Param>::parameters( QVector<C> &param_vector, uint strt ) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    if (this->hasMissingParameters())
        throw SireDB::missing_parameter(QObject::tr(
                        "Cannot copy parameters as some are missing..."), CODELOC);

    //is the vector of the right size?
    uint last_index = this->nAtoms() + strt;

    if (param_vector.isEmpty())
        //resize the vector ourselves
        param_vector.resize(last_index);
    else if ( uint(param_vector.size()) < last_index )
        throw SireError::invalid_arg(QObject::tr(
                "Cannot copy the parameters into the passed vector as it is the wrong "
                "size. It has size() == %1, but needs to have at least size() == %2.")
                    .arg(param_vector.size()).arg(last_index), CODELOC);

    //ok - lets copy the parameters, one CutGroup at a time
    //append all of the parameters for each CutGroup in turn
    QVector<CutGroupID> cgroups = this->orderedCutGroupIDs();
    const CutGroupID *array = cgroups.constData();

    int ncg = cgroups.count();
    for (int i=0; i<ncg; ++i)
    {
        //get the parameters in this CutGroup and copy them into
        //the array starting at index 'strt' - this returns the
        //index of the start point for the next CutGroup
        strt = params.parameters( array[i], param_vector, strt );
    }

    return strt;
}

/** Copy all of the parameters for residue 'resnum' into the vector 'param_vector'.
    This works identically to 'parameters(resnum)' except that it copies the parameters
    into an existing vector, starting at index 'strt'. This vector must either be
    sufficiently large to hold the parameters (e.g. size() >= nAtoms(resnum) + strt)
    or it must be empty, in which case it will be automatically dimensioned to
    be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    This returns the index that is one past the index of the last parameter
    that is copied into the array

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
uint AtomTableT<Param>::parameters( ResNum resnum, QVector<C> &param_vector,
                                    uint strt ) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    if (this->hasMissingParameters(resnum))
        throw SireDB::missing_parameter(QObject::tr(
                    "Cannot copy parameters for residue %1 as some are missing...")
                        .arg(resnum.toString()), CODELOC);

    //get the info object describing this residue
    const ResidueCGInfo &resinfo = this->info().residue(resnum);

    uint nats = resinfo.nAtoms();

    //is the vector of the right size?
    uint last_index = nats + strt;

    if (param_vector.isEmpty())
        //resize the vector ourselves
        param_vector.resize(last_index);
    else if ( uint(param_vector.size()) < last_index )
        throw SireError::invalid_arg(QObject::tr(
                "Cannot copy the parameters into the passed vector as it is the wrong "
                "size. It has size() == %1, but needs to have at least size() == %2.")
                    .arg(param_vector.size()).arg(last_index), CODELOC);

    //get a raw pointer to the array
    C *array = param_vector.data();

    //ok - lets copy the parameters
    for (uint i=0; i<nats; ++i)
    {
        array[ i+strt ] = params[ resinfo[i] ];
    }

    return last_index;
}

/** Copy all of the parameters for the CutGroup with ID == cgid into the vector
    'param_vector'. This works identically to 'parameters(cgid)' except that it
    copies the parameters into an existing vector, starting at index 'strt'.
    This vector must either be sufficiently large to hold the parameters
    (e.g. size() >= nAtoms(cgid) + strt) or it must be empty, in which case
    it will be automatically dimensioned to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    This returns the index that is one past the index of the last parameter
    that is copied into the array

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
uint AtomTableT<Param>::parameters( CutGroupID cgid, QVector<C> &param_vector,
                                    uint strt ) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    if (this->hasMissingParameters(cgid))
        throw SireDB::missing_parameter(QObject::tr(
                        "Cannot copy parameters for CutGroup %1 as some are missing...")
                            .arg(cgid.toString()), CODELOC);

    //is the vector of the right size?
    uint last_index = this->nAtoms(cgid) + strt;

    if (param_vector.isEmpty())
        //resize the vector ourselves
        param_vector.resize(last_index);
    else if ( uint(param_vector.size()) < last_index )
        throw SireError::invalid_arg(QObject::tr(
                "Cannot copy the parameters into the passed vector as it is the wrong "
                "size. It has size() == %1, but needs to have at least size() == %2.")
                    .arg(param_vector.size()).arg(last_index), CODELOC);

    //ok - lets copy the parameters for this CutGroup
    strt = params.parameters( cgid, param_vector, strt );

    //strt and last_index must now be equal, or my book-keeping is wrong :-)
    BOOST_ASSERT(strt == last_index);

    return last_index;
}

/** Copy all of the parameters for the CutGroups whose IDs are in cgids
    into the GroupedVector 'param_vector'.  This works identically to
    'parametersByCutGroup(cgids)' except that it copies the parameters
    into an existing vector.

    This vector must either be exactly the right size to hold the parameters
    or it must be empty, in which case it will be automatically dimensioned
    to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    \throw SireError::invalid_arg
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::parametersByCutGroup(const QSet<CutGroupID> &cgids,
                                      GroupedVector<CGAtomID,C> &param_vector) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        if (this->hasMissingParameters(*it))
            throw SireDB::missing_parameter(QObject::tr(
                        "Cannot copy parameters as some are missing..."), CODELOC);
    }

    //now check that all of the sizes are correct....
    if (not param_vector.isEmpty())
    {
        for (typename GroupedVector<CGAtomID,C>::const_iterator
                                                it = param_vector.constBegin();
             it != param_vector.constEnd();
             ++it)
        {
            if (cgids.contains(it.key()) and it->size() != this->nAtoms(it.key()))
                throw SireError::invalid_arg(QObject::tr("Cannot copy the parameters "
                         "as the passed GroupedVector is of the wrong size. The vector "
                         "for CutGroup %1 has size() == %2, while it has to have "
                         "size() == %3.")
                            .arg(it.key()).arg(it->size()).arg(this->nAtoms(it.key())),
                                CODELOC);
        }
    }

    //everything is ok - we can now copy the parameters :-)
    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        CutGroupID cgid = *it;

        //is there space for the parameters in this CutGroup?
        if (param_vector.contains(cgid))
        {
            //there is - copy in the parameters
            params.parameters(cgid, param_vector[cgid]);
        }
        else
        {
            //create the vector and dimension it to the right size
            QVector<C> group_params( this->nAtoms(cgid) );
            params.parameters(cgid, group_params);

            //insert it into the GroupedVector
            param_vector.insert(cgid, group_params);
        }
    }
}

/** Copy all of the parameters for the residueus whose numbers are
    in 'resnums' into the GroupedVector 'param_vector'.
    This works identically to 'parametersByResidue(resnums)' except that it
    copies the parameters into an existing vector.

    This vector must either be exactly the right size to hold the parameters
    or it must be empty, in which case it will be automatically dimensioned
    to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::parametersByResidue(const QSet<ResNum> &resnums,
                                    GroupedVector<ResNumAtomID,C> &param_vector) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        if (this->hasMissingParameters(*it))
            throw SireDB::missing_parameter(QObject::tr(
                        "Cannot copy parameters as some are missing..."), CODELOC);
    }

    //now check that all of the sizes are correct....
    if (not param_vector.isEmpty())
    {
        for (typename GroupedVector<ResNumAtomID,C>::const_iterator
                                                    it = param_vector.constBegin();
             it != param_vector.constEnd();
             ++it)
        {
            if (resnums.contains(it.key()) and it->size() != this->nAtoms(it.key()))
                throw SireError::invalid_arg(QObject::tr("Cannot copy the parameters "
                         "as the passed GroupedVector is of the wrong size. The vector "
                         "for residue number %1 has size() == %2, while it has to have "
                         "size() == %3.")
                            .arg(it.key()).arg(it->size()).arg(this->nAtoms(it.key())),
                                CODELOC);
        }
    }

    //everything is ok - we can now copy the parameters :-)
    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        ResNum resnum = *it;

        //is there space for the parameters in this residue?
        if (param_vector.contains(resnum))
        {
            //there is - copy in the parameters
            this->parameters(resnum, param_vector[resnum]);
        }
        else
        {
            //create the vector (the call to 'parameters' will
            //correctly dimension it)
            QVector<C> res_params;
            this->parameters(resnum, res_params);

            //insert it into the GroupedVector
            param_vector.insert(resnum, res_params);
        }
    }
}

/** Copy all of the parameters into the GroupedVector 'param_vector'.
    This works identically to 'parametersByCutGroup()' except that it
    copies the parameters into an existing vector.

    This vector must either be exactly the right size to hold the parameters
    or it must be empty, in which case it will be automatically dimensioned
    to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    \throw SireError::invalid_arg
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::parametersByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const
{
    //are there any missing parameters? If there are then just throw an
    //exception
    if (this->hasMissingParameters())
        throw SireDB::missing_parameter(QObject::tr(
                        "Cannot copy parameters as some are missing..."), CODELOC);

    //now check that all of the sizes are correct....
    if (not param_vector.isEmpty())
    {
        for (typename GroupedVector<CGAtomID,C>::const_iterator
                                        it = param_vector.constBegin();
             it != param_vector.constEnd();
             ++it)
        {
            if (it->size() != this->nAtoms(it.key()))
                throw SireError::invalid_arg(QObject::tr("Cannot copy the parameters "
                         "as the passed GroupedVector is of the wrong size. The vector "
                         "for group %1 has size() == %2, while it has to have "
                         "size() == %3.")
                            .arg(it.key()).arg(it->size()).arg(this->nAtoms(it.key())),
                                CODELOC);
        }
    }

    //everything is ok - we can now copy the parameters :-)
    QVector<CutGroupID> cgroups = this->orderedCutGroupIDs();
    int ngroups = cgroups.count();
    const CutGroupID *array = cgroups.constData();

    for (int i=0; i<ngroups; ++i)
    {
        CutGroupID cgid = array[i];

        //is there space for the parameters for this group?
        if (param_vector.contains(cgid))
        {
            //there is - copy in the parameters :-)
            params.parameters(cgid, param_vector[cgid]);
        }
        else
        {
            //create the vector of the right size
            QVector<C> group_params( this->nAtoms(cgid) );

            //fill it with the parameters
            params.parameters(cgid, group_params);

            //insert it into the GroupedVector
            param_vector.insert(cgid, group_params);
        }
    }
}

/** Copy all of the parameters into the GroupedVector 'param_vector'.
    This works identically to 'parametersByResidue()' except that it
    copies the parameters into an existing vector.

    This vector must either be exactly the right size to hold the parameters
    or it must be empty, in which case it will be automatically dimensioned
    to be of the right size.

    Note that if any exceptions are thrown then 'param_vector' is not modified.

    \throw SireError::invalid_arg
    \throw SireDB::missing_parameter
*/
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AtomTableT<Param>::parametersByResidue(
                                    GroupedVector<ResNumAtomID,C> &param_vector ) const
{
    this->parametersByResidue( this->residueNumbers(), param_vector );
}

static const SireStream::MagicID atomtablet_magic
                        = SireStream::getMagic("SireDB::AtomTableT");

}

/** Serialise an AtomTable to a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<T> &table)
{
    SireStream::writeHeader(ds, SireDB::atomtablet_magic, 1)
               << table.params << static_cast<const SireDB::AtomTable&>(table);

    return ds;
}

/** Deserialise an AtomTable from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<T> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireDB::atomtablet_magic,
                                                     "SireDB::AtomTableT");

    if (v == 1)
    {
        ds >> table.params >> static_cast<SireDB::AtomTable&>(table);
    }
    else
        throw SireStream::version_error(v, "1", "SireDB::AtomTableT", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
