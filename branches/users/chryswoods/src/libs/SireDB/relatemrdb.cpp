
#include "relatemrdb.h"
#include "namedb.h"
#include "matchmr.h"
#include "relatemrdata.h"

#include "SireError/errors.h"
#include "SireDB/errors.h"

#include <QSqlQuery>
#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireStream;

static const RegisterMetaType<RelateMRDB> r_relatemrdb("SireDB::RelateMRDB", NO_STREAM);

const QString moleculeGroups = "moleculeGroups";
const QString moleculeStates = "moleculeStates";
const QString residueAliases = "residueAliases";

/** Constructor */
RelateMRDB::RelateMRDB() : DBBase(), namedb(0)
{}

/** Copy constructor */
RelateMRDB::RelateMRDB(const RelateMRDB &other) : DBBase(other), namedb(0)
{}

/** Destructor */
RelateMRDB::~RelateMRDB()
{}

/** Return whether or not molecule group matching is case-sensitive
    (by default it is not) */
bool RelateMRDB::caseSensitiveMoleculeGroups() const
{
    return namedb->caseSensitive(moleculeGroups);
}

/** Return whether or not molecule state matching is case-sensitive
    (by default it is not) */
bool RelateMRDB::caseSensitiveMoleculeStates() const
{
    return namedb->caseSensitive(moleculeStates);
}

/** Tell the database to use case-sensitive matching of molecule groups.
    Note that changing the case-sensitivity between loading parameter
    data and assigning the molecule may lead to weird results and is not
    recommended! */
void RelateMRDB::setCaseSensitiveMoleculeGroups()
{
    namedb->setCaseSensitive(moleculeGroups);
}

/** Tell the database to use case-sensitive matching of molecule states.
    Note that changing the case-sensitivity between loading parameter
    data and assigning the molecule may lead to weird results and is not
    recommended! */
void RelateMRDB::setCaseSensitiveMoleculeStates()
{
    namedb->setCaseSensitive(moleculeStates);
}
    
/** Tell the database to use case-insensitive matching of molecule groups.
    Note that changing the case-sensitivity between loading parameter
    data and assigning the molecule may lead to weird results and is not
    recommended! */
void RelateMRDB::setCaseInsensitiveMoleculeGroups()
{
    namedb->setCaseInsensitive(moleculeGroups);
}
    
/** Tell the database to use case-insensitive matching of molecule states.
    Note that changing the case-sensitivity between loading parameter
    data and assigning the molecule may lead to weird results and is not
    recommended! */
void RelateMRDB::setCaseInsensitiveMoleculeStates()
{
    namedb->setCaseInsensitive(moleculeStates);
}

/** Tell the database to use case-sensitive matching of residue alias strings */
void RelateMRDB::setCaseSensitiveResidueAliases()
{
    namedb->setCaseSensitive( residueAliases );
}

/** Tell the database to use case-insensitive matching of residue alias strings
    (default) */
void RelateMRDB::setCaseInsensitiveResidueAliases()
{
    namedb->setCaseInsensitive( residueAliases );
}

/** Return whether case-sensitive matching is used when searching for residue 
    alias strings */
bool RelateMRDB::caseSensitiveResidueAliases()
{
    return namedb->caseSensitive( residueAliases );
}

/** Create the tables used by this database */
void RelateMRDB::initialise()
{
    DBBase::initialise();

    //this needs a NameDB to work - ask the parent to create a NameDB
    parent().addComponent<SireDB::NameDB>();
    namedb = &(parent().asA<SireDB::NameDB>());
    
    QStringList columns;
    
    // Create the table that holds the complete set of molecule and residue 
    // matching criteria and maps it to a unique relationship ID number
    QString errors = executeSQL( "create table 'SireDB_RelateMRDB_relate' ("
                      "MolName INTEGER, MolGroup INTEGER, MolState INTEGER, ResAlias INTEGER, "
                      "ResName INTEGER, ResNum INTEGER, ResBonds TEXT, ResNoBonds TEXT, "
                      "RelateID INTEGER, Score INTEGER, PRIMARY KEY(RelateID) )");
              
    // Create the table that maps the set of residue matching criteria to 
    // a residue alias string - this allows multiple residues to share 
    // matching criteria (e.g. all of the backbone atoms of protein residues)
    errors += executeSQL( "create table 'SireDB_RelateMRDB_resalias' ("
                      "ResAlias INTEGER, ResName INTEGER, ResNum INTEGER, "
                      "ResBonds TEXT, ResNoBonds TEXT )" );
              
    if (not errors.isNull())
        throw SireDB::db_error(errors, CODELOC);
}                           
    
/** Dump the version number of this database */
void RelateMRDB::prepareToDump()
{
    DBBase::prepareToDump();
    
    this->saveParameter<RelateMRDB>( "version", 1 );
}

/** Check the version number */
void RelateMRDB::postLoad()
{
    DBBase::postLoad();

    int v = this->loadParameter<RelateMRDB>( "version" ).toInt();
                            
    if (v != 1)
        throw version_error( v, "1", "SireDB::RelateMRDB", CODELOC );
}

/** Add a molecule group to the database and return the NameID that it is assigned. 
    Note that if the molecule group is already in the database then the NameID
    of the existing entry will be returned. */
NameID RelateMRDB::addMoleculeGroup(const QString &name)
{
    return namedb->addName( moleculeGroups, name );
}

/** Return the NameID of molecule group 'name', or 0 if there is no such molecule in the 
    database */
NameID RelateMRDB::getMoleculeGroupID(const QString &name)
{
    return namedb->getNameID( moleculeGroups, name );
}

/** Return the name of the molecule group with NameID 'id', or a null string if there is
    no such name in the database */
QString RelateMRDB::getMoleculeGroup(NameID id)
{
    return namedb->getName( moleculeGroups, id );
}

/** Add a molecule state to the database and return the NameID that it is assigned. 
    Note that if the molecule state is already in the database then the NameID
    of the existing entry will be returned. */
NameID RelateMRDB::addMoleculeState(const QString &name)
{
    return namedb->addName( moleculeStates, name );
}

/** Return the NameID of molecule state 'molstate', or 0 if there is no such molecule in the 
    database */
NameID RelateMRDB::getMoleculeStateID(const QString &name)
{
    return namedb->getNameID( moleculeStates, name );
}

/** Return the name of the molecule state with NameID 'id', or a null string if there is
    no such name in the database */
QString RelateMRDB::getMoleculeState(NameID id)
{
    return namedb->getName( moleculeStates, id );
}

/** Add a residue alias and return the identifying NameID number */
NameID RelateMRDB::addResidueAlias(const QString &resalias)
{
    return namedb->addName( residueAliases, resalias );
}

/** Return the NameID number of the residue alias 'resalias', or 0 if 
    it is not in the database */
NameID RelateMRDB::getResidueAliasID(const QString &resalias)
{
    return namedb->getNameID( residueAliases, resalias );
}

/** Return the residue alias with NameID 'id', or an empty string if there
    is no such alias in the database, or if id == 0 */
QString RelateMRDB::getResidueAlias(NameID id)
{
    return namedb->getName( residueAliases, id );
}

/** Process a single match and translate it into data that may be
    used in the database */
RelateMRData RelateMRDB::convert(const MatchMR &match)
{
    //get the ID numbers of the names contained in the match
    NameID molname = namedb->addMoleculeName(match.molecule().name());
    NameID molgroup = this->addMoleculeGroup(match.molecule().group());
    NameID molstate = this->addMoleculeState(match.molecule().state());
    NameID resname = namedb->addResidueName(match.residue().name());
    NameID resalias = this->addResidueAlias(match.residue().alias());
    
    //convert the names of atoms involved in interresidue bonds to 
    //a list of NameIDs of the atoms, which is then converted to 
    //a string of the IDs (a space between each ID)
    NameIDSet resbonds;
    foreach (QString atom, match.residue().resResBonds())
        resbonds.insert( namedb->addAtomName(atom) );
        
    //do the same for the names of atoms not involved in interresidue
    //bonds
    NameIDSet resnobonds;
    foreach (QString atom, match.residue().noResResBonds())
        resnobonds.insert( namedb->addAtomName(atom) );
        
    return RelateMRData( molname, molgroup, molstate, resalias, resname, 
                         match.residue().number(), resbonds, resnobonds );
}

/** Convert the data that is understood by the database back into a format that
    is better understood by the user (a MatchMR) */
MatchMR RelateMRDB::convert(const RelateMRData &match)
{
    MatchMol moldata;
    MatchRes resdata;
    
    //get the names of the parts of this match
    moldata.setName( namedb->getMoleculeName( match.moleculeName() ) );
    moldata.setGroup( this->getMoleculeGroup( match.moleculeGroup() ) );
    moldata.setState( this->getMoleculeState( match.moleculeState() ) );
    resdata.setName( namedb->getResidueName( match.residueName() ) );
    resdata.setNumber( match.residueNumber() );
    resdata.setAlias( this->getResidueAlias( match.residueAlias() ) );
    
    //convert the NameIDs of bonded atoms back into a list of atom names
    foreach (NameID atom, match.bondedAtoms())
        resdata.addResResBond( namedb->getAtomName(atom) );
        
    //do the same for the NameIDs of atoms that are not involved in bonding
    foreach (NameID atom, match.nonBondedAtoms())
        resdata.addNoResResBond( namedb->getAtomName(atom) );
        
    return MatchMR(moldata, resdata);
}

/** Get the ID number of the relationship encoded into the match data 'relatedata'.
    Returns 0 if there is no such relationship in the database. */
RelateID RelateMRDB::get(const RelateMRData &relatedata)
{
    if (relatedata.isEmpty())
        return 0;
    else
    {
        //search this table for the relationship
        QSqlQuery q(database());
    
        q.exec( QString("select RelateID from 'SireDB_RelateMRDB_relate' where ( %1 )")
                                      .arg(relatedata.toQueryString()) );
        checkErrors(q,CODELOC);
    
        q.next();
        if (q.isValid())
            //we've found a match :-)
            return q.value(0).toUInt();
        else
            //no luck - its not in the database
            return 0;
    }
}

/** Add the relationship matched via 'match'. This returns the unique ID number 
    of this relationship in the database.  */
tuple<RelateID,int> RelateMRDB::add(const MatchMR &match)
{
    //if this is an empty match then return 0
    if (match.isEmpty())
        return tuple<RelateID,int>(0,0);
    else
    {
        //convert the match into a format that may be understood by 
        //the database
        RelateMRData relatedata = convert(match);

        //search for this match in the database
        RelateID relateid = get(relatedata);
    
        if (relateid == 0)
        {
            //there is no match - insert this into the database
        
            //get a unique relationship ID number
            relateid = getNewRelateID();
        
            QSqlQuery q(database());
            q.exec( QString("insert into 'SireDB_RelateMRDB_relate' values ( %1, %2, %3 )")
                        .arg(relatedata.toInsertString())
                        .arg(relateid).arg(relatedata.score()) );
            checkErrors(q,CODELOC);
        }
  
        return tuple<RelateID,int>(relateid, relatedata.score());
    }
}

/** Return the ID number of the relationship specified by 'match'. 
    This returns '0' if there is no such relationship in the database. */
tuple<RelateID,int> RelateMRDB::get(const MatchMR &match)
{
   if (match.isEmpty())
      return tuple<RelateID,int>(0,0);
   else
   {
      //convert the match into a format that may be understood
      //by the database
      RelateMRData relatedata = convert(match);
      return tuple<RelateID,int>( get(relatedata), relatedata.score() );
   }
}

/** Associate the residue matching criteria in 'resdata' to the alias 'alias' */
void RelateMRDB::alias(const MatchRes &resdata, const QString &alias)
{
    //get the NameID of the residue's name
    NameID resname = namedb->addResidueName( resdata.name() );
    
    //get the residue's number
    ResNum resnum = resdata.number();

    //get the NameID of the alias
    NameID resalias = this->addResidueAlias(alias);

    //convert the names of atoms involved in interresidue bonds to 
    //a list of NameIDs of the atoms, which is then converted to 
    //a string of the IDs (a space between each ID)
    NameIDSet atomnames;
    foreach (QString atom, resdata.resResBonds())
        atomnames.insert( namedb->addAtomName(atom) );
        
    QString resbonds = convertToString(atomnames);
    atomnames.clear();
        
    //do the same for the names of atoms not involved in interresidue
    //bonds
    foreach (QString atom, resdata.noResResBonds())
        atomnames.insert( namedb->addAtomName(atom) );
        
    QString resnobonds = convertToString(atomnames);
        
    //does the database already contain this alias?
    QSqlQuery q(database());
    q.exec( QString("select ResAlias from 'SireDB_RelateMRDB_resalias' where "
                    "(ResAlias = %1 and ResName = %2 and ResNum = %3 and "
                     "ResBonds = \"%4\" and ResNoBonds = \"%5\")")
              .arg(resalias).arg(resname).arg(resnum.toString(), resbonds, resnobonds) );
    checkErrors(q, CODELOC);
    
    q.next();
    if (not q.isValid())
    {
        //the alias is not in the database - we need to add it
        q.exec( QString("insert into 'SireDB_RelateMRDB_resalias' values "
                        "( %1, %2, %3, \"%4\", \"%5\" )")
                  .arg(resalias).arg(resname).arg(resnum.toString(), resbonds, resnobonds) );
        checkErrors(q, CODELOC);
    }
    
}

/** Search for all relationships that match the data in 'relatedata'. The matches
    are indexed by score, and an empty has is returned if there are no matching 
    relationships. */
RelateIDMap RelateMRDB::search(const RelateMRData &relatedata)
{
    //get the list of aliases that this residue may go by...
    NameIDSet resaliases;
    
    QSqlQuery q(database());

    q.exec( QString("select ResAlias, ResBonds, ResNoBonds from 'SireDB_RelateMRDB_resalias' "
                    "where (ResName = %1 or ResName = 0) and (ResNum = %2 or ResNum = 0)")
                .arg(relatedata.residueName()).arg(relatedata.residueNumber().toString()) );
    checkErrors(q, CODELOC);
    
    for (q.next(); q.isValid(); q.next())
    {
        QString bonds = q.value(1).toString();
        QString nobonds = q.value(2).toString();
        
        //convert the bonding strings to NameIDSets...
        NameIDSet resbonds = convertToNameIDSet(bonds);
        NameIDSet resnobonds = convertToNameIDSet(nobonds);
        
        //only save this match if the bonding requirements are met by this query..
        if (relatedata.hasCompatibleBonds(resbonds,resnobonds))
        {
            //insert the ID number of the relationship, indexed by the score
            resaliases.insert( q.value(0).toUInt() );
        }
    }
    
    //use these aliases together with the search data in 'relatedata' to 
    //find all of the matching relationships for the molecule and residue
    //criteria
    q.exec( QString("select RelateID, Score, ResBonds, ResNoBonds from "
                    "'SireDB_RelateMRDB_relate' where %1")
                .arg(relatedata.toSearchString(resaliases)) );
    checkErrors(q,CODELOC);
    
    //process each match - need to check that the bonding of the match
    //is compatible with what we have...
    RelateIDMap matches;
    
    for (q.next(); q.isValid(); q.next())
    {
        QString bonds = q.value(2).toString();
        QString nobonds = q.value(3).toString();
        
        //convert the bonding strings to NameIDSets...
        NameIDSet resbonds = convertToNameIDSet(bonds);
        NameIDSet resnobonds = convertToNameIDSet(nobonds);
        
        //only save this match if the bonding requirements are met by this query..
        if (relatedata.hasCompatibleBonds(resbonds,resnobonds))
        {
            //insert the ID number of the relationship, indexed by the score
            matches.insert( q.value(1).toInt(), q.value(0).toUInt() );
        }
    }
    
    //return the matches
    return matches;
}

/** Search for all relationships that match the data in 'match' for matching 'n' 
    atoms. The matches are sorted by score (most specific match first), and
    an empty list is returned if there are no matching relationships */
RelateIDMap RelateMRDB::search(const MatchMR &match)
{
    return search( convert(match) );
}

/** Return the match for the RelateID 'relateid' - returns an empty match 
    if either this isn't in the database, or if relateid == 0 */
MatchMR RelateMRDB::get(RelateID relateid)
{
    if (relateid == 0)
        return MatchMR();
    else
    {
        QSqlQuery q(database());
        q.exec( QString("select MolName, MolGroup, MolState, "
                        "ResAlias, ResName, ResNum, ResBonds, ResNoBonds "
                        "from 'SireDB_RelateMRDB_relate' where RelateID = %1").arg(relateid) );
        checkErrors(q, CODELOC);
        
        q.next();
        if (q.isValid())
        {
            //we have a match!
            return convert( RelateMRData( q.value(0).toUInt(),  //molname
                                          q.value(1).toUInt(),  //molgroup
                                          q.value(2).toUInt(),  //molstate
                                          q.value(3).toUInt(),  //resalias
                                          q.value(4).toUInt(),  //resname
                                          q.value(5).toUInt(),  //resnum
                                          convertToNameIDSet(q.value(6).toString()), //resbonds
                                          convertToNameIDSet(q.value(7).toString()) //resnobonds
                                        )
                          );
        }
        else
        {
            //no match :-(
            addLog( QString("WARNING: Could not find RelateMRDB relationship "
                            "with RelateID == %1").arg(relateid) );
            return MatchMR();
        }
    }
}
