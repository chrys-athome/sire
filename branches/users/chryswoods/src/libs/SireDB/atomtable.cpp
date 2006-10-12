
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "atomtable.h"

#include "SireMol/moleculeinfo.h"

#include "SireMol/atom.h"
#include "SireMol/cutgroup.h"

#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireDB;

static const RegisterMetaType<AtomTable> r_atomtable("SireDB::AtomTable", MAGIC_ONLY);

/** Serialise an AtomTable to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomTable &table)
{
    writeHeader(ds, r_atomtable, 1)
            << static_cast<const TableBase&>(table);

    return ds;
}

/** Deserialise an AtomTable from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomTable &table)
{
    VersionID v = readHeader(ds, r_atomtable);

    if (v == 1)
    {
        ds >> static_cast<TableBase&>(table);
    }
    else
        throw version_error( v, "1", r_atomtable, CODELOC );

    return ds;
}

/** Constructor */
AtomTable::AtomTable() : TableBase()
{}

/** Construct an AtomTable to hold the parameters for the passed molecule */
AtomTable::AtomTable(const MoleculeInfo &moleculeinfo)
          : TableBase(moleculeinfo)
{}

/** Copy constructor - this is fast as the data of this class is
    implicitly shared */
AtomTable::AtomTable(const AtomTable &other)
          : TableBase(other)
{}

/** Destructor */
AtomTable::~AtomTable()
{}

/** Return whether or not this is an empty table (contains no atoms) */
bool AtomTable::isEmpty() const
{
    return info().isEmpty();
}

/** Return the total number of parameters in this table
    (this includes missing parameters, thus
    nParameters() = nAssignedParameters() + nMissingParameters()
*/
int AtomTable::nParameters() const
{
    //the number of parameters must be the same as the number
    //of atoms
    return info().nAtoms();
}

/** Return the total number of parameters in this table
    that are in the residue with number 'resnum'.

    (this includes missing parameters, thus
    nParameters(resnum) = nAssignedParameters(resnum) + nMissingParameters(resnum)

    \throw SireMol::missing_residue
*/
int AtomTable::nParameters(ResNum resnum) const
{
    //the number of parameters must be the same as the number
    //of atoms
    return info().nAtoms(resnum);
}

/** Return the total number of parameters in this table
    that are in the residue with index 'resid'.

    (this includes missing parameters, thus
    nParameters(resid) = nAssignedParameters(resid) + nMissingParameters(resid)

    \throw SireError::invalid_index
*/
int AtomTable::nParameters(ResID resid) const
{
    //the number of parameters must be the same as the number
    //of atoms
    return info().nAtoms(resid);
}

/** Return the total number of parameters in this table
    that are in the CutGroup with ID == cgid.

    (this includes missing parameters, thus
    nParameters(cgid) = nAssignedParameters(cgid) + nMissingParameters(cgid)

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nParameters(CutGroupID cgid) const
{
    //the number of parameters must be the same as the number
    //of atoms
    return info().nAtoms(cgid);
}

/** Return the total number of parameters in this table
    that are in the CutGroup with number 'cgnum'.

    (this includes missing parameters, thus
    nParameters(cgnum) = nAssignedParameters(cgnum) + nMissingParameters(cgnum)

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nParameters(CutGroupNum cgnum) const
{
    //the number of parameters must be the same as the number
    //of atoms
    return info().nAtoms(cgnum);
}

/** Return the total number of assigned parameters in this table */
int AtomTable::nAssignedParameters() const
{
    int ncg = info().nCutGroups();

    int nparams = 0;
    for (CutGroupID i(0); i<ncg; ++i)
        nparams += this->_unsafe_nAssignedParameters(i);

    return nparams;
}

/** Return the total number of assigned parameters in the
    residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int AtomTable::nAssignedParameters(ResNum resnum) const
{
    return this->_unsafe_nAssignedParameters( info().at(resnum) );
}

/** Return the total number of assigned parameters in the
    residue at index 'resid'

    \throw SireError::invalid_index
*/
int AtomTable::nAssignedParameters(ResID resid) const
{
    return this->_unsafe_nAssignedParameters( info().at(resid) );
}

/** Return the total number of assigned parameters in the
    CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nAssignedParameters(CutGroupID cgid) const
{
    info().assertCutGroupExists(cgid);
    return this->_unsafe_nAssignedParameters(cgid);
}

/** Return the total number of assigned parameters in the
    CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nAssignedParameters(CutGroupNum cgnum) const
{
    return this->_unsafe_nAssignedParameters( info().cutGroupID(cgnum) );
}

/** Return the total number of missing parameters */
int AtomTable::nMissingParameters() const
{
    int ncg = info().nCutGroups();

    int nparams = 0;
    for (CutGroupID i(0); i<ncg; ++i)
        nparams += this->_unsafe_nMissingParameters(i);

    return nparams;
}

/** Return the total number of missing parameters in the residue
    with number 'resnum'

    \throw SireMol::missing_residue
*/
int AtomTable::nMissingParameters(ResNum resnum) const
{
    return this->_unsafe_nMissingParameters( info().at(resnum) );
}

/** Return the total number of missing parameters in the residue
    with index 'resid'

    \throw SireError::invalid_index
*/
int AtomTable::nMissingParameters(ResID resid) const
{
    return this->_unsafe_nMissingParameters( info().at(resid) );
}

/** Return the total number of missing parameters in the CutGroup
    with ID == cgid

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nMissingParameters(CutGroupID cgid) const
{
    info().assertCutGroupExists(cgid);
    return this->_unsafe_nMissingParameters( cgid );
}

/** Return the total number of missing parameters in the CutGroup
    with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
int AtomTable::nMissingParameters(CutGroupNum cgnum) const
{
    return this->_unsafe_nMissingParameters( info().cutGroupID(cgnum) );
}

/** Return whether or not the parameter for the atom with
    index 'cgatomid' has been assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomTable::assignedParameter(const CGAtomID &cgatomid) const
{
    info().assertAtomExists(cgatomid);
    return this->_unsafe_assignedParameter(cgatomid);
}

/** Return whether or not the parameter for the atom with
    index 'cgatomid' has been assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomTable::assignedParameter(const CGNumAtomID &cgatomid) const
{
    return this->_unsafe_assignedParameter( info().at(cgatomid) );
}

/** Return whether or not the parameter for the atom with
    index 'resatomid' has been assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomTable::assignedParameter(const ResNumAtomID &resatomid) const
{
    return this->_unsafe_assignedParameter( info().at(resatomid) );
}

/** Return whether or not the parameter for the atom with
    index 'resatomid' has been assigned.

    \throw SireError::invalid_index
*/
bool AtomTable::assignedParameter(const ResIDAtomID &resatomid) const
{
    return this->_unsafe_assignedParameter( info().at(resatomid) );
}

/** Return whether or not the parameter for the atom with
    AtomIndex 'atom' has been assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
bool AtomTable::assignedParameter(const AtomIndex &atom) const
{
    return this->_unsafe_assignedParameter( info().at(atom) );
}

/** Return whether or not the parameter for the atom with
    index 'atomid' has been assigned.

    \throw SireError::invalid_index
*/
bool AtomTable::assignedParameter(AtomID atomid) const
{
    return this->_unsafe_assignedParameter( info().at(atomid) );
}

/** Return whether or not there are any missing parameters
    in this table */
bool AtomTable::hasMissingParameters() const
{
    int ncg = info().nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
        if (this->_unsafe_hasMissingParameters(i))
            return true;

    return false;
}

/** Return whether or not the residue with number 'resnum' has
    any missing parameters in this table

    \throw SireMol::missing_residue
*/
bool AtomTable::hasMissingParameters(ResNum resnum) const
{
    return this->_unsafe_hasMissingParameters( info().at(resnum) );
}

/** Return whether or not the residue with index 'resid' has
    any missing parameters in this table

    \throw SireError::invalid_index
*/
bool AtomTable::hasMissingParameters(ResID resid) const
{
    return this->_unsafe_hasMissingParameters( info().at(resid) );
}

/** Return whether the CutGroup with ID == cgid has any
    missing parameters in this table

    \throw SireMol::missing_cutgroup
*/
bool AtomTable::hasMissingParameters(CutGroupID cgid) const
{
    info().assertCutGroupExists(cgid);
    return this->_unsafe_hasMissingParameters(cgid);
}

/** Return whether the CutGroup with number 'cgnum' has any
    missing parameters in this table

    \throw SireMol::missing_cutgroup
*/
bool AtomTable::hasMissingParameters(CutGroupNum cgnum) const
{
    return this->_unsafe_hasMissingParameters( info().cutGroupID(cgnum) );
}

/** Return the set of AtomIndexes of all atoms that are missing
    parameters in this table */
QSet<AtomIndex> AtomTable::missingParameters() const
{
    int ncg = info().nCutGroups();

    QSet<AtomIndex> atms;

    for (CutGroupID i(0); i<ncg; ++i)
        atms += this->_unsafe_missingParameters(i);

    return atms;
}

/** Return the set of AtomIndex objects of all atoms in the residue
    with number 'resnum' that are missing parameters in this table

    \throw SireMol::missing_residue
*/
QSet<AtomIndex> AtomTable::missingParameters(ResNum resnum) const
{
    return this->_unsafe_missingParameters( info().at(resnum) );
}

/** Return the set of AtomIndex objects of all atoms in the residue
    with index 'resid' that are missing parameters in this table

    \throw SireError::invalid_index
*/
QSet<AtomIndex> AtomTable::missingParameters(ResID resid) const
{
    return this->_unsafe_missingParameters( info().at(resid) );
}

/** Return the set of AtomIndex objects of all atoms in the CutGroup
    with ID == cgid that are missing parameters in this table

    \throw SireMol::missing_cutgroup
*/
QSet<AtomIndex> AtomTable::missingParameters(CutGroupID cgid) const
{
    info().assertCutGroupExists(cgid);
    return this->_unsafe_missingParameters(cgid);
}

/** Return the set of AtomIndex objects of all atoms in the CutGroup
    with number 'cgnum' that are missing parameters in this table

    \throw SireMol::missing_cutgroup
*/
QSet<AtomIndex> AtomTable::missingParameters(CutGroupNum cgnum) const
{
    return this->_unsafe_missingParameters( info().cutGroupID(cgnum) );
}

/** Clear this table of all parameters. Note that this only
    clears the values of the parameters - it does not remove
    the atoms */
void AtomTable::clear()
{
    int ncg = info().nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
        this->_unsafe_clear(i);
}

/** Clear this table of the parameter for the atom at
    index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void AtomTable::clear(const CGAtomID &cgatomid)
{
    info().assertAtomExists(cgatomid);
    this->_unsafe_clear(cgatomid);
}

/** Clear this table of the parameter for the atom at
    index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void AtomTable::clear(const CGNumAtomID &cgatomid)
{
    this->_unsafe_clear( info().at(cgatomid) );
}

/** Clear this table of the parameter for the atom at
    index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void AtomTable::clear(const ResNumAtomID &resatomid)
{
    this->_unsafe_clear( info().at(resatomid) );
}

/** Clear this table of the parameter for the atom at
    index 'resatomid'

    \throw SireError::invalid_index
*/
void AtomTable::clear(const ResIDAtomID &resatomid)
{
    this->_unsafe_clear( info().at(resatomid) );
}

/** Clear this table of the parameter for the atom with
    AtomIndex 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void AtomTable::clear(const AtomIndex &atom)
{
    this->_unsafe_clear( info().at(atom) );
}

/** Clear this table of the parameter for the atom at
    index 'atomid'

    \throw SireError::invalid_index
*/
void AtomTable::clear(AtomID atomid)
{
    this->_unsafe_clear( info().at(atomid) );
}

/** Clear this table of all parameters for the residue
    with number 'resnum'

    \throw SireMol::missing_residue
*/
void AtomTable::clear(ResNum resnum)
{
    this->_unsafe_clear( info().at(resnum) );
}

/** Clear this table of all parameters for the residue
    at index 'resid'

    \throw SireError::invalid_index
*/
void AtomTable::clear(ResID resid)
{
    this->_unsafe_clear( info().at(resid) );
}

/** Clear this table of all parameters for the atoms
    in the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
void AtomTable::clear(CutGroupID cgid)
{
    info().assertCutGroupExists(cgid);
    this->_unsafe_clear(cgid);
}

/** Clear this table of all parameters for the atoms
    in the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
void AtomTable::clear(CutGroupNum cgnum)
{
    this->_unsafe_clear( info().cutGroupID(cgnum) );
}
