#ifndef SIREMM_ASSIGN_INTERNALS_HPP
#define SIREMM_ASSIGN_INTERNALS_HPP

#include <QSet>
#include <QDebug>

#include "SireDB/assignbase.h"
#include "SireDB/parametertable.h"
#include "SireDB/parameterdb.h"
#include "SireDB/relationshipdb.h"

#include "SireBase/dynamicsharedptr.hpp"
#include "SireStream/datastream.h"

#include "internaldb.hpp"
#include "internalgenerator.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Internals>
class assign_internals;
}

template<class Internals>
QDataStream& operator<<(QDataStream&, const SireMM::assign_internals<Internals>&);
template<class Internals>
QDataStream& operator>>(QDataStream&, SireMM::assign_internals<Internals>&);

namespace SireMM
{
using SireDB::DBBase;
using SireDB::using_database;
using SireDB::MatchMRData;
using SireDB::AssignBase;
using SireDB::ParameterTable;
using SireDB::ParameterDB;
using SireDB::RelationshipDB;

using SireBase::DynamicSharedPtr;

/** This is the base class of all of the assign_internals type classes 
    (e.g. assign_bonds, assign_angles and assign_dihedrals). This allows
    the common code to be extracted into a single location
    
    @author Christopher Woods
*/
template<class Internals>
class SIREMM_EXPORT assign_internals : public AssignBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const assign_internals<Internals>&);
friend QDataStream& ::operator>><>(QDataStream&, assign_internals<Internals>&);

public:
    typedef typename Internals::internal_type internal_type;
    typedef internal_type Internal;

    assign_internals(const InternalGenerator<Internals> &generator);
    assign_internals(const QSet<Internal> &internals,
                     const InternalGenerator<Internals> &generator);
    
    assign_internals(const assign_internals &other);
    
    ~assign_internals();
    
protected:
    void assignInternals( const Molecule &molecule,
                          ParameterTable &param_table, 
                          ParameterDB &database,
                          const MatchMRData &matchmr ) const;

private:
    /** The generator that is used to generate internals for new
        parameter tables */
    DynamicSharedPtr< InternalGenerator<Internals> > generator;
    
    /** The set of internals to assign */
    QSet<Internal> internals_to_assign;
};

/** Construct to parametise all of the internals in the molecule, using 
    'internal_generator' to generate the internals of new tables */
template<class Internals>
assign_internals<Internals>::assign_internals(const InternalGenerator<Internals>
                                                                    &internal_generator)
                            : generator( internal_generator )
{}

/** Construct to only parametise the internals in 'internals', using 
    'internal_generator' to generate the internals of new tables */
template<class Internals>
assign_internals<Internals>::assign_internals(
                                  const QSet<typename Internals::internal_type> &internals,
                                  const InternalGenerator<Internals> &internal_generator)
                            : AssignBase(),
                              generator( internal_generator ),
                              internals_to_assign(internals)
{}

/** Copy constructor */
template<class Internals>
assign_internals<Internals>::assign_internals(const assign_internals &other)
                            : AssignBase(other), 
                              generator(other.generator),
                              internals_to_assign(other.internals_to_assign)
{}

/** Destructor */
template<class Internals>
assign_internals<Internals>::~assign_internals()
{}

/** This private class is used as a workspace by assign_internals during
    the assignment of internal parameters. This class should not be used by
    any other code except for assign_internals.
    
    @author Christopher Woods
*/
template<class Internals>
class assign_internals_ws
{
public:
    typedef InternalDB<Internals> db_type;
    typedef InternalTableBase<Internals> table_type;

    typedef typename Internals::internal_type internal_type;
    typedef internal_type Internal;

    assign_internals_ws(ParameterTable &param_table, 
                        const MatchMRData &matchmr);
    
    ~assign_internals_ws();
    
    void append(db_type &internaldb, table_type &internaltable);
    void append(RelationshipDB &relatedb);
    
    void assignParameters(bool overwrite);
    
    void assignParameters(const Internal &internal, bool overwrite,
                          bool skipmissing=false);

private:

    ParameterTable &param_table;
    const MatchMRData &matchmr;

    QList<db_type*> paramdbs;
    QList<table_type*> tables;
    QList<RelationshipDB*> relatedbs;
};

/** Constructor */
template<class Internals>
assign_internals_ws<Internals>::assign_internals_ws(ParameterTable &table, 
                                                    const MatchMRData &match)
                               : param_table(table), matchmr(match)
{}

/** Destructor */
template<class Internals>
assign_internals_ws<Internals>::~assign_internals_ws()
{}

/** Add an internal database with corresponding internal table to the workspace. */
template<class Internals>
void assign_internals_ws<Internals>::append( 
                             typename assign_internals_ws<Internals>::db_type &db,
                             typename assign_internals_ws<Internals>::table_type &table )
{
    paramdbs.append( &db );
    tables.append( &table );
}

/** Add a relationship database to the workspace */
template<class Internals>
void assign_internals_ws<Internals>::append( RelationshipDB &relatedb )
{
    relatedbs.append( &relatedb );
}

/** Internal function used to assign the parameters of only a 
    single internal */
template<class Internals>
void assign_internals_ws<Internals>::assignParameters(
                                            const typename Internals::internal_type &internal, 
                                            bool overwrite, bool skipmissing)
{
    //create a record of whether or not the parameters for this internal
    //have been found during the lifetime of this function
    int nparams = paramdbs.count();
    QSet<int> found_param;
        
    //now record all of the tables that have already had parameters assigned
    if (skipmissing)
    {
        for (int i=0; i<nparams; ++i)
        {
            table_type &table = *(tables[i]);
            
            if (not table.info().contains(internal))
                //we have 'found' the parameter if this table does not contain
                //space for this parameters :-)
                found_param.insert(i);
            else if ( (not overwrite) and table.assignedParameter(internal) )
                found_param.insert(i);
        }
    }
    else if (not overwrite)
    {
        for (int i=0; i<nparams; ++i)
        {
            if (tables.at(i)->assignedParameter(internal))
                found_param.insert(i);
        }
    }
    
    //have we assigned all of the parameters for this internal - if so then
    //the size of found_param will be the same as paramdbs
    if (found_param.count() == nparams)
        return;
        
    //ok - we now know that we have to find at least some of the parameters 
    //for this atom. Do this by looping over each relationship database in turn...
    int nrelates = relatedbs.count();
    
    for (int i=0; i<nrelates; ++i)
    {
        //get the RelateIDMap for this internal in the molecule from 
        //this relationship database...
        
        RelateIDMap relateids = relatedbs[i]->search(internal, param_table, matchmr);
        
        for (int j=0; j<nparams; ++j)
        {
            if (not found_param.contains(j))
            {
                if ( paramdbs[j]->assignParameter(internal, relateids, param_table) )
                    found_param.insert(j);
            }
            
            //have we assigned all of the parameters for this internal yet?
            if (found_param.count() == nparams)
                return;
        }
    }
}

/** Internal function used to assign the parameters of all of the internals
    in each of the parameter tables */
template<class Internals>
void assign_internals_ws<Internals>::assignParameters(bool overwrite)
{
    //record the set of internals whose parameters have been assigned
    QSet<Internal> already_assigned;

    //loop over each parameter table
    int nparams = tables.count();
    
    for (int i=0; i<nparams; ++i)
    {
        table_type &table = *(tables[i]);
    
        //loop over each internal in the table
        const Internals &internals = table.info();
        
        for (typename Internals::const_iterator it = internals.begin();
             it.isValid();
             ++it)
        {
            const Internal &internal = it.key();
            
            //only assign this internal if it has not already been assigned
            if (not already_assigned.contains(internal))
            {
                //assign this internal in each of the tables that contain it
                assignParameters(internal, overwrite, true);
                
                //if there is more than one parameter table then record
                //that this internal has already been assigned in all of the parameter
                //tables (prevents reassigning it when we go round this loop again)
                if (nparams > 1)
                    already_assigned.insert(internal);
            }
        }
    }
}

/** Assign the parameters in the table 'param_table' using 
    the database 'database' according to the requirements contained in this object.
    If 'overWrite()' is true then the parameters will be overwritten, 
    otherwise only parameters for internals that are currently missing 
    parameters will be found. 
    
    If a generator (or list of internals) was passed to the constructor
    of this object then only the internals that were generated (or specified)
    will be assigned.
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Internals>
void assign_internals<Internals>::assignInternals( const Molecule &molecule,
                                                   ParameterTable &param_table,
                                                   ParameterDB &database,
                                                   const MatchMRData &matchmr ) const
{
    //maintain the invariant by working in a copy of 'param_table' and then 
    //only copying the results back into 'param_table' at the end of the assignment
    ParameterTable local_param_table = param_table;

    //create a workspace for this assignment operation
    assign_internals_ws<Internals> workspace(local_param_table, matchmr);

    typedef typename assign_internals_ws<Internals>::db_type db_type;
    typedef typename assign_internals_ws<Internals>::table_type table_type;

    //loop through each of the requested databases and see whether or
    //not they are present in the database... (we do this now to prevent
    //lots of polymorphic lookups for each and every internal!)
    QList<db_type*> dbs_missing_tables;
    
/*    foreach (QString paramdb, parameterDataBases())
    {
        if (database.isA(paramdb))
        {
            //cast the database as this component
            DBBase &dbpart = database.asA(paramdb);
            
            //is this the right database type?
            if ( dbpart.isA<db_type>() )
            {
                //get the InternalDB
                db_type &internaldb = dbpart.asA<db_type>();
                
                if (internaldb.hasTable(local_param_table))
                    workspace.append(internaldb, internaldb.getTable(local_param_table));
                else
                    //the table for this parameter needs to be created
                    dbs_missing_tables.append( &internaldb );
            }
        }
    }
    
    //do any of the parameter tables need to be created? - we can only
    //create tables if we have a generator
    if (not dbs_missing_tables.isEmpty())
    {
        BOOST_ASSERT( generator.constData() != 0 );
    
        Internals generated_internals( molecule,
                                       *generator );
                                       
        //now go through each database and create the missing table
        foreach( db_type *dbptr, dbs_missing_tables )
        {
            workspace.append( *dbptr, dbptr->createTable(local_param_table,
                                                         generated_internals) );
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
    
    //if we have a list of internals to assign, then assign each one in turn
    if (not internals_to_assign.isEmpty())
    {
        //iterate over all of the internals in 'generated_internals'
        for (typename QSet<Internal>::const_iterator it = internals_to_assign.constBegin();
             it != internals_to_assign.constEnd();
             ++it)
        {
            //assign the parameters for this internal
            workspace.assignParameters( *it, overwriteParameters() );
        }
    }
    else
    {
        //have to assign the parameters one-by-one (as different tables may
        //have different internals)
        workspace.assignParameters( overwriteParameters() );
    }*/
    
    //all of the parameters have been assigned into our local copy of the 
    //parameter table - now copy it back to the original
    param_table = local_param_table;
}

const SireStream::MagicID assign_internals_magic = SireStream::getMagic(
                                                          "SireMM::assign_internals");

}

/** Serialise to a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireMM::assign_internals<Internals> &assigner)
{
    SireStream::writeHeader(ds, SireMM::assign_internals_magic, 1)
             << assigner.generator << assigner.internals_to_assign
             << static_cast<const SireDB::AssignBase&>(assigner);
             
    return ds;
}

/** Deserialise from a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, 
                        SireMM::assign_internals<Internals> &assigner)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::assign_internals_magic,
                                                     "SireMM::assign_internals");
                                                     
    if (v == 1)
    {
        ds >> assigner.generator >> assigner.internals_to_assign
           >> static_cast<SireDB::AssignBase&>(assigner);
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::AssignBase", CODELOC);
        
    return ds;
}

SIRE_END_HEADER

#endif
