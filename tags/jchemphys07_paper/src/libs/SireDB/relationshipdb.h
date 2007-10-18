/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREDB_RELATIONSHIPDB_H
#define SIREDB_RELATIONSHIPDB_H

#include <QList>

#include "dbbase.h"
#include "matchmrdata.h"
#include "parametertable.h"

#include "SireMol/atomindex.h"
#include "SireMol/molecule.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using SireMol::AtomIndex;

/**
This is the base class of all relationship databases. This contains no data - just an interface that is used by the assign classes (e.g. assign_atoms)

@author Christopher Woods
*/
class SIREDB_EXPORT RelationshipDB : public DBBase
{
public:
    RelationshipDB();
    RelationshipDB(const RelationshipDB &other);

    ~RelationshipDB();

    static const char* typeName()
    {
        return "SireDB::RelationshipDB";
    }

    template<class T>
    RelateIDMap search( const T &internal,
                        const Molecule &molecule,
                        const ParameterTable &param_table,
                        const MatchMRData &matchmr = MatchMRData() );

protected:

    /** Pure virtual function used to return the RelateIDMap containing
        the ranked RelateIDs of relationships that match the atoms in 'atoms',
        that are in the molecule 'molecule', and whose parameters are in 'param_table'.
        Note that this function does not need to sanity check these arguments, as they
        have already been checked by the 'search' function that calls this
        virtual function. This function should only throw an exception in
        extreme circumstances, as failure to find a relationship is not an error! */
    virtual RelateIDMap findMatches(const QList<AtomIndex> &atoms,
                                    const Molecule &molecule,
                                    const ParameterTable &param_table,
                                    const MatchMRData &matchmr)=0;

    void prepareToDump();
    void postLoad();
};

/** Return the relationship IDs that match the internal 'internal', using
    the information that is currently stored in the parameter table 'param_table'.

    Note that this function does all of the sanity checking for the search so
    that the derived child classes don't need to.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
RelateIDMap RelationshipDB::search(const T &internal,
                                   const Molecule &molecule,
                                   const ParameterTable &param_table,
                                   const MatchMRData &matchmr)
{
    //ensure that the table and molecule are compatible
    param_table.assertCompatibleWith(molecule);

    //convert the internal into a list of atoms
    QList<AtomIndex> atoms;

    for (int i=0; i<internal.size(); ++i)
    {
        molecule.info().assertAtomExists( internal[i] );
        atoms.append( internal[i] );
    }

    return findMatches(atoms, molecule, param_table, matchmr);
}

/** Specialisation for search(atom) */
template<>
inline RelateIDMap RelationshipDB::search(const AtomIndex &atom,
                                   const Molecule &molecule,
                                   const ParameterTable &param_table,
                                   const MatchMRData &matchmr)
{
    //ensure that the table and molecule are compatible
    param_table.assertCompatibleWith(molecule);

    //ensure that this atom exists!
    molecule.info().assertAtomExists(atom);

    QList<AtomIndex> atoms;
    atoms.append( atom );

    return findMatches(atoms, molecule, param_table, matchmr);
}

}

SIRE_END_HEADER

#endif
