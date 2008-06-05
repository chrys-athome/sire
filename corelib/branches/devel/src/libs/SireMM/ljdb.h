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

#ifndef SIREMM_LJDB_H
#define SIREMM_LJDB_H

#include "SireDB/atomdb.h"

#include "ljparameter.h"
#include "ljtable.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireDB::ParamID;
using SireDB::RelateID;
using SireDB::AssertMatch;
using SireDB::RelateIDMap;
using SireDB::ParameterTable;

/**
This forcefield database component is used to store Lennard Jones parameters, used to calculate the vdw interactions.

@author Christopher Woods
*/
class SIREMM_EXPORT LJDB : public SireDB::AtomDB
{
public:
    /** The LJDB stores LJParameter objects */
    typedef LJParameter parameter_type;

    /** A LJDB will place its parameters into an LJTable */
    typedef LJTable table_type;

    LJDB();
    LJDB(const LJDB &other);

    ~LJDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::LJDB";
    }

    const char* what() const
    {
        return LJDB::typeName();
    }

    void addLJ(const QString &userid, const LJParameter &ljparam);
    LJParameter getLJ(const QString &userid, bool *foundlj = 0);

    void relateLJ(const AssertMatch<1> &matchatom, const QString &userid);
    void relateLJ(const AssertMatch<1> &matchatom, const LJParameter &ljparam);

    void relateLJ(RelateID relateid, const QString &userid);
    void relateLJ(RelateID relateid, const LJParameter &ljparam);

    LJParameter getLJ(RelateID relateid, bool *foundlj = 0);
    LJParameter getLJ(const RelateIDMap &relateids, bool *foundlj = 0);

    void createTable( ParameterTable &param_table ) const;

    bool assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                          ParameterTable &param_table );

protected:
    void initialise();
    void prepareToDump();
    void postLoad();

    ParamID addLJ(const LJParameter &ljparam);
    LJParameter retrieveLJ(ParamID id);
};

}

Q_DECLARE_METATYPE(SireMM::LJDB)

SIRE_END_HEADER

#endif
