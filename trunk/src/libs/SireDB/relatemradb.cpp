
#include "relatemradb.h"
#include "matchmra.h"
#include "matchmradata.h"
#include "relatemradata.h"
#include "namedb.h"
#include "relatemrdb.h"
#include "relatemrdata.h"
#include "parametertable.h"

#include "SireDB/errors.h"

#include <QSqlQuery>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType< QSet<quint32> > r_qset_quint32_("QSet<quint32>");
Q_DECLARE_METATYPE(QSet<quint32>)

static const RegisterMetaType<RelateMRADB> r_relatemradb("SireDB::RelateMRADB", NO_STREAM);

/** Constructor */
RelateMRADB::RelateMRADB() 
            : RelationshipDB(), relatemrdb(0), namedb(0)
{}

/** Copy constructor */
RelateMRADB::RelateMRADB(const RelateMRADB &other) 
            : RelationshipDB(other), relatemrdb(0), namedb(0)
{}

/** Destructor */
RelateMRADB::~RelateMRADB()
{}

/** Initialise the RelateMRADB */
void RelateMRADB::initialise()
{
    //this component needs to use a NameDB and a RelateMRDB
    parent().addComponent<SireDB::NameDB>();
    parent().addComponent<SireDB::RelateMRDB>();
    
    //get pointers to these components
    relatemrdb = &(parent().asA<RelateMRDB>());
    namedb = &(parent().asA<NameDB>());
}

/** Save the current state of this DB */
void RelateMRADB::prepareToDump()
{
    RelationshipDB::prepareToDump();
    
    saveParameter<RelateMRADB>( "version", 1 );
    saveParameter<RelateMRADB>( "relatetables", QVariant::fromValue(relatetables) );
}

/** Load up the state of this DB */
void RelateMRADB::postLoad()
{
    RelationshipDB::postLoad();
    
    int v = loadParameter<RelateMRADB>( "version" ).toInt();
    
    if (v == 1)
    {
        relatetables = loadParameter<RelateMRADB>( "relatetables" )
                                                    .value< QSet<quint32> >();
    }
    else
        throw version_error( v, "1", "SireDB::RelateMRADB", CODELOC );
}

/** Return the name of the relationship table for 'n' atoms */
QString RelateMRADB::relateTable(uint n)
{
    return QString("SireDB_RelateMRADB_relate_%1").arg(n);
}

/** Create the relationship table used to hold relationships of groups of 'n' atoms */
void RelateMRADB::createRelateTable(uint n)
{
    if (n == 0 or relatetables.contains(n))
        return;

    //get the name of the table
    QString tablename = relateTable(n);
    
    QStringList columns;
    
    for (uint i=0; i<n; ++i)
        columns.append( QString("MolResID%1 INTEGER, AtomName%1 INTEGER, "
                                "AtomNum%1 INTEGER").arg(i) );
    
    QString error = executeSQL(QString(
          "CREATE TABLE '%1' ( %2, RelateID INTEGER, Score INTEGER, PRIMARY KEY(RelateID) )")
              .arg(tablename, columns.join(", ")) );
              
    if (not error.isNull())
        throw SireDB::db_error(error, CODELOC);
        
    relatetables.insert(n);
}

/** Process a single match and translate it into data that may be
    used in the database */
RelateMRAData RelateMRADB::convert(const MatchMRA &match)
{
    //get the RelateID and score of the molecule and residue criteria
    tuple<RelateID,int> idscore = relatemrdb->add( match.molAndRes() );
    
    //now convert the name of the atom into a NameID number
    NameID atomid = namedb->addAtomName( match.atom().name() );
    
    return RelateMRAData( idscore, atomid, match.atom().number() );
}

/** Convert 'match' for 'n' atoms into a format that may be understood by 
    the database */
RelateMRADataList RelateMRADB::convert(const MatchMRAData &match, uint n)
{
    //convert the criteria for each atom into a format that can
    //be added to this database
    RelateMRADataList relatedata;
    
    for (uint i=0; i<n; ++i)
        relatedata.append( convert(match.getMatch(i)) );

    return relatedata;
}

/** Convert 'match' from a format suitable for the database (RelateMRAData) back
    into a more user-friendly format (MatchMRA) */
MatchMRA RelateMRADB::convert(const RelateMRAData &match)
{
    //first, convert the molecule and residue parts...
    MatchMR molresdata = relatemrdb->get( match.molResID() );
    
    //now convert back the atom name
    MatchAtom atomdata;
    atomdata.setName( namedb->getAtomName( match.atomName() ) );
    atomdata.setNumber( match.atomNumber() );
    
    return MatchMRA(molresdata, atomdata);
}

/** Convert 'match' for multiple atoms from a format suitable for use in the database 
    back into a more user-friendly format */
MatchMRAData RelateMRADB::convert(const RelateMRADataList &match)
{
    MatchMRAData ret;
    
    uint n = match.count();
    for (uint i=0; i<n; ++i)
        ret.set( i, convert(match.at(i)) );
        
    //simplify the match data (e.g. collapse together common parts)
    ret.simplify(n);
        
    return ret;
}

/** Get the ID number of the relationship encoded into the match data 'relatedata'.
    Returns 0 if there is no such relationship in the database. */
RelateID RelateMRADB::get(const RelateMRADataList &relatedata)
{
    //get the number of atoms matched by this data
    uint n = relatedata.count();
    
    //is there a table of relationships for this number of atoms?
    if (not relatetables.contains(n))
        return 0;
        
    //the table exists, so lets get its name
    QString tablename = relateTable(n);
    
    //search this table for the relationship
    QSqlQuery q(database());
    
    //create the query string for the data...
    QStringList querystrings;
    for( uint i=0; i<n; ++i )
        querystrings.append( relatedata.at(i).toQueryString(i) );
    
    q.exec( QString("select RelateID from '%1' where ( %2 )")
              .arg(tablename, querystrings.join(" and ")) );
    checkErrors(q,CODELOC);
    
    q.next();
    if (q.isValid())
        //we've found a match :-)
        return q.value(0).toUInt();
    else
        //no luck - its not in the database
        return 0;
}

/** Add the relationship matched via 'match' that involves matching 'n' atoms 
    (n == 1 for a single atom, n == 2 for a pair of atoms/bond etc.). This
    returns the unique ID number of this relationship in the database. This
    allows this relationship to be associated with a parameter. */
RelateID RelateMRADB::add(const MatchMRAData &match, uint n)
{
    if (n == 0 or match.isEmpty())
        return 0;
        
    //convert the match into a format that may be understood by 
    //the database
    RelateMRADataList relatedata = convert(match,n);

    return add(relatedata);
}

/** Add the relationship matched via 'match' that involves matching 'n' atoms 
    (n == 1 for a single atom, n == 2 for a pair of atoms/bond etc.). This
    returns the unique ID number of this relationship in the database. This
    allows this relationship to be associated with a parameter. */
RelateID RelateMRADB::add(const MatchMRA &match)
{
    if (match.isEmpty())
        return 0;
        
    //convert the match into a format that may be understood by the database
    RelateMRAData relatedata = convert(match);
    RelateMRADataList list;
    list.append(relatedata);
    
    return add(list);
}

/** Internal function used to add a relationship (in 'relatedata') to 
    the database, and to return the ID number of the relationship. */
RelateID RelateMRADB::add(const RelateMRADataList &relatedata)
{
    uint n = relatedata.count();

    //search for this match in the database
    RelateID relateid = get(relatedata);
    
    if (relateid == 0)
    {
        //there is no match - insert this into the database
        QString tablename = relateTable(n);
        QStringList insertstrings;
        int score = 0;
        
        for( uint i=0; i<n; ++i )
        {
            const RelateMRAData &data = relatedata.at(i);
            insertstrings.append( data.toInsertString() );
            
            //also calculate the score of the match
            score += data.score();
        }
        
        //we may need to create the table for this number of atoms...
        if (not relatetables.contains(n))
            createRelateTable(n);
        
        //get a unique relationship ID number
        relateid = this->getNewRelateID();
        
        QSqlQuery q(database());
        q.exec( QString("insert into '%1' values ( %2, %3, %4 )")
                    .arg(tablename, insertstrings.join(", "))
                    .arg(relateid).arg(score) );
        checkErrors(q,CODELOC);
    }

    return relateid;
}

/** Return the ID number of the relationship specified by 'match', and 
    matching 'n' atoms. This returns '0' if there is no such relationship
    in the database. */
RelateID RelateMRADB::get(const MatchMRAData &match, uint n)
{
    if (n == 0 or match.isEmpty())
        return 0;
    else
    {
        //convert the match into a format that may be understood
        //by the database
        return get( convert(match,n) );
    }
}

/** Return the ID number of the relationship specified by 'match', and 
    matching 'n' atoms. This returns '0' if there is no such relationship
    in the database. */
RelateID RelateMRADB::get(const MatchMRA &match)
{
    if (match.isEmpty())
        return 0;
    else
    {
        //convert the match into a format that may be understood
        //by the database
        RelateMRAData relatedata = convert(match);
        RelateMRADataList list;
        list.append(relatedata);
        
        return get( list );
    }
}

/** Private function used by RelateMRADB::search to convert the RelateIDs of 
    the matching molecule and residue criteria into a string suitable for 
    making a database query */
QString RelateMRADB::queryString(const RelateIDMap &molresmap, uint i)
{
    QStringList parts;
    
    for (RelateIDMap::const_iterator it = molresmap.begin();
         it != molresmap.end();
         ++it)
    {
        if (it.value() != 0)
            parts.append( QString("MolResID%1 = %2").arg(i).arg(it.value()) );
    }
    
    if (parts.isEmpty())
        return QString("MolResID%1 = 0").arg(i);
    else
        return QString("%1 or MolResID%2 = 0").arg( parts.join(" or ") ).arg(i);
}

/** Search for all relationships that match the data in 'match' for matching 'n' 
    atoms. The matches are sorted by score (most specific match first), and
    an empty list is returned if there are no matching relationships */
RelateIDMap RelateMRADB::findMatches(const MatchMRAData &match, uint n)
{
    if (n == 0 or not relatetables.contains(n))
        return RelateIDMap();
    else
    {
        QStringList querystrings;
    
        //get each of the n matches that form the complete match...
        for (uint i=0; i<n; ++i)
        {
            MatchMRA m = match.getMatch(i);
            
            //now search for all of the matching molecule and residue matches...
            RelateIDMap molresmatch = relatemrdb->search(m.molAndRes());
            
            //get the name ID of the atom name
            NameID atomid = namedb->addAtomName(m.atom().name());
            
            //convert these into a query string
            querystrings.append( QString("(AtomName%1 = %2 or AtomName%1 = 0) and "
                                         "(AtomNum%1 = %3 or AtomNum%1 = 0) and "
                                         "(%4)")
                                  .arg(i)
                                  .arg(atomid)
                                  .arg(m.atom().number().toString(),
                                       queryString(molresmatch,i))
                               );
        }
        
        //now perform the search
        QSqlQuery q(database());
        
        q.exec( QString("select RelateID, Score from '%1' where (%2)")
                  .arg(relateTable(n), querystrings.join(" and ")) );
        checkErrors(q, CODELOC);
        
        RelateIDMap matches;
        
        for (q.next(); q.isValid(); q.next())
        {
            matches.insert( q.value(1).toInt(), q.value(0).toUInt() );
        }
        
        return matches;
    }
}

/** Search for all relationships that match the list of atoms in 'atoms'
    based on the information stored in the parameter table 'param_table'.
    
    The matches are sorted by score (most specific match first), and
    an empty list is returned if there are no matching relationships 
*/
RelateIDMap RelateMRADB::findMatches(const QList<AtomIndex> &atoms,
                                     const ParameterTable &param_table,
                                     const MatchMRData &matchmr)
{
    //create a MatchMRA for each atom
    const Molecule &mol = param_table.molecule();
    
    MatchMol matchmol;
    matchmol.setName( mol.name() );
    
    MatchMRAData matchdata;
    
    int nats = atoms.count();
    for (int i=0; i<nats; ++i)
    {
        const Atom &atom = param_table.molecule().atom( atoms[i] );
    
        MatchAtom matchatom;
        matchatom.setName( atom.name() );
        matchatom.setNumber( atom.number() );
        
        MatchRes matchres;
        matchres.setName( mol.residueName(atom.resNum()) );
        matchres.setNumber( atom.resNum() );
        
        matchdata.set( i, MatchMRA(matchmol, matchres, matchatom) );
    }
    
    //combine the match data from the atoms with the match data passed
    //by the user
    matchdata = matchmr and matchdata;
    
    return this->findMatches(matchdata, nats);
}

/** Return the MatchMRAData that corresponds to the relationship ID 'relateid'
    for matching a group of 'n' atoms. Returns a completely empty match if 
    there is no such relationship (or if relateid == 0) */
MatchMRAData RelateMRADB::get(RelateID relateid, uint n)
{
    if (n == 0 or relateid == 0 or not relatetables.contains(n))
        return MatchMRAData();
    else
    {
        QString tablename = relateTable(n);
        
        QStringList parts;
        for (uint i=0; i<n; ++i)
        {
            parts.append( QString("MolResID%1, AtomName%1, AtomNum%1").arg(i) );
        }
        
        QSqlQuery q(database());
        
        q.exec( QString("select %1 from '%2' where RelateID = %3")
                    .arg(parts.join(", "), tablename).arg(relateid) );
        checkErrors(q, CODELOC);
        
        q.next();
        if (q.isValid())
        {
            //we have a match!
            RelateMRADataList match;
            
            for (uint i=0; i<n; ++i)
            {
                uint p = 3*i;
            
                match.append( RelateMRAData( q.value(p+0).toUInt(),  //MolResID%i
                                             q.value(p+1).toUInt(),  //AtomName%i
                                             q.value(p+2).toUInt() ) ); //AtomNum%i
            }
            
            return convert(match);
        }
        else
        {
            //relationship was not found
            addLog( QString("WARNING: Could not find RelateMRADB relationship with "
                            "RelateID == %1 for n == %2").arg(relateid).arg(n) );
            return MatchMRAData();
        }
    }
}
