
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "assign_atoms.h"
#include "matchmrdata.h"
#include "parameterdb.h"
#include "parametertable.h"
#include "atomdb.h"
#include "atomtable.h"
#include "relationshipdb.h"

#include "SireMol/cutgroup.h"
#include "SireMol/cutgroupid.h"
#include "SireMol/atom.h"
#include "SireMol/atomid.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;

static const RegisterMetaType<assign_atoms> r_assign_atoms("SireDB::assign_atoms");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::assign_atoms &atoms)
{
    writeHeader(ds, r_assign_atoms, 1)
            << atoms.atoms_to_be_parametised
            << static_cast<const AssignBase&>(atoms);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::assign_atoms &atoms)
{
    VersionID v = readHeader(ds, r_assign_atoms);

    if (v == 1)
    {
        ds >> atoms.atoms_to_be_parametised
           >> static_cast<AssignBase&>(atoms);
    }
    else
        throw version_error(v, "1", r_assign_atoms, CODELOC);

    return ds;
}

/** Null constructor */
assign_atoms::assign_atoms() : AssignBase()
{}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0
                          ) : AssignBase()
{
    addDataBase(db0);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the atoms using the ten sets of requirements specified
    (given that each 'using_database' can contain at least 10 databases,
     this can be a lot of requirements!) */
assign_atoms::assign_atoms(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9
                          ) : AssignBase()
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Assign only the atoms in 'atms' */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms)
             : AssignBase(), atoms_to_be_parametised(atms)
{}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the atoms using the requirements specified */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the atoms using the ten sets of requirements specified
    (given that each 'using_database' can contain at least 10 databases,
     this can be a lot of requirements!) */
assign_atoms::assign_atoms(const QSet<AtomIndex> &atms,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9
                          ) : AssignBase(), atoms_to_be_parametised(atms)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Copy constructor */
assign_atoms::assign_atoms(const assign_atoms &other)
             : AssignBase(other), atoms_to_be_parametised(other.atoms_to_be_parametised)
{}

/** Destructor */
assign_atoms::~assign_atoms()
{}

/** This is a private internal class that is used to provide the
    workspace for assign_atoms to perform the actual assignments */
class assign_atoms_ws
{
public:
    assign_atoms_ws(ParameterTable &param_table,
                    const MatchMRData &matchmr);

    ~assign_atoms_ws();

    void append(AtomDB &atomdb, AtomTable &atomtable);
    void append(RelationshipDB &relatedb);

    void assignParameters(const AtomIndex &atom, bool overwrite);

private:

    ParameterTable &param_table;
    const MatchMRData &matchmr;

    QList<AtomDB*> paramdbs;
    QList<AtomTable*> tables;
    QList<RelationshipDB*> relatedbs;
};

/** Constructor */
assign_atoms_ws::assign_atoms_ws(ParameterTable &table, const MatchMRData &match)
                : param_table(table), matchmr(match)
{}

/** Destructor */
assign_atoms_ws::~assign_atoms_ws()
{}

/** Add an atom database with corresponding atom table to the workspace. */
void assign_atoms_ws::append( AtomDB &atomdb, AtomTable &atomtable )
{
    paramdbs.append( &atomdb );
    tables.append( &atomtable );
}

/** Add a relationship database to the workspace */
void assign_atoms_ws::append( RelationshipDB &relatedb )
{
    relatedbs.append( &relatedb );
}

/** Internal function used to assign the parameters of only a
    single atom */
void assign_atoms_ws::assignParameters(const AtomIndex &atom, bool overwrite)
{
    //create a record of whether or not the parameters for this atom
    //have been found during the lifetime of this function
    int nparams = paramdbs.count();
    QSet<int> found_param;

    //now record all of the tables that have already had parameters assigned
    if ( not overwrite )
    {
        for (int i=0; i<nparams; ++i)
        {
            if (tables.at(i)->assignedParameter(atom))
                found_param.insert(i);
        }
    }

    //have we assigned all of the parameters for this atom - if so then
    //the size of found_param will be the same as paramdbs
    if (found_param.count() == nparams)
        return;

    //ok - we now know that we have to find at least some of the parameters
    //for this atom. Do this by looping over each relationship database in turn...
    int nrelates = relatedbs.count();

    for (int i=0; i<nrelates; ++i)
    {
        //get the RelateIDMap for this atom in the molecule from
        //this relationship database...

        RelateIDMap relateids = relatedbs[i]->search(atom, param_table, matchmr);

        if (not relateids.isEmpty())
        {
            for (int j=0; j<nparams; ++j)
            {
                if (not found_param.contains(j))
                {
                    if ( paramdbs[j]->assignParameter(atom, relateids, param_table) )
                        found_param.insert(j);
                }

                //have we assigned all of the parameters for this atom yet?
                if (found_param.count() == nparams)
                    return;
            }
        }
    }
}

/** Assign the parameters in the table 'param_table' using
    the database 'database' according to the requirements contained in this object.
    If 'overwrite' is true then the parameters will be overwritten,
    otherwise only parameters for atoms that are currently missing
    parameters will be found. */
void assign_atoms::assignParameters( ParameterTable &param_table,
                                     ParameterDB &database,
                                     const MatchMRData &matchmr ) const
{
    //create a workspace for this assignment operation
    assign_atoms_ws workspace(param_table, matchmr);

    const Molecule &molecule = param_table.molecule();

    //loop through each of the requested databases and see whether or
    //not they are present in the database... (we do this now to prevent
    //lots of polymorphic lookups for each and every atom!)
    foreach (QString paramdb, parameterDataBases())
    {
        if (database.isA(paramdb))
        {
            //cast the database as this component
            DBBase &dbpart = database.asA(paramdb);

            //is this an AtomDB? (must be to hold atom parameters)
            if (dbpart.isA<AtomDB>())
            {
                //get the AtomDB
                AtomDB &atomdb = dbpart.asA<AtomDB>();

                //now get the AtomTable that will hold the parameters
                AtomTable &atomtable = atomdb.createTable(param_table);

                //save these references to the list of parameters dbs to hunt
                workspace.append(atomdb, atomtable);
            }
        }
    }

    //loop through the requested relationship databases and add those to the workspace
    foreach (QString relatedb, relationshipDataBases())
    {
        if (database.isA(relatedb))
        {
            //cast the database as this component
            DBBase &dbpart = database.asA(relatedb);

            //is this a RelationshipDB? (must be to hold relationships)
            if (dbpart.isA<RelationshipDB>())
            {
                //get the RelationshipDB
                RelationshipDB &relatedb = dbpart.asA<RelationshipDB>();

                //save this reference
                workspace.append(relatedb);
            }
        }
    }

    if ( atoms_to_be_parametised.isEmpty() )
    {
        //try to parametise all of the atoms in the molecule
        for (CutGroupID i(0); i<molecule.nCutGroups(); ++i)
        {
            CutGroup cgroup = molecule.cutGroup(i);

            for (AtomID j(0); j<cgroup.nAtoms(); ++j)
            {
                workspace.assignParameters(cgroup[j], overwriteParameters());
            }
        }
    }
    else
    {
        //only try to parametise the atoms in 'atoms_to_be_parametised'
        for (QSet<AtomIndex>::const_iterator it = atoms_to_be_parametised.constBegin();
             it != atoms_to_be_parametised.constEnd();
             ++it)
        {
            workspace.assignParameters(*it, overwriteParameters());
        }
    }
}
