
#include "SireMol/qhash_siremol.h"

#include "relateatomtypedb.h"
#include "relatemrdb.h"
#include "namedb.h"
#include "relateatomtypedata.h"
#include "matchmr.h"
#include "matchatomtype.h"
#include "matchatomtypedata.h"
#include "parametertable.h"
#include "atomtypetable.h"

#include "SireMol/residuebonds.h"

#include "SireError/errors.h"
#include "SireDB/errors.h"

#include <QSqlQuery>
#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;
using namespace SireStream;

const QString atomTypes = "AtomTypes";

Q_DECLARE_METATYPE(QSet<quint32>)

static const RegisterMetaType<RelateAtomTypeDB> r_relateatdb(NO_STREAM);

/** Constructor */
RelateAtomTypeDB::RelateAtomTypeDB()
                 : RelationshipDB(), relatemrdb(0), namedb(0)
{}

/** Copy constructor */
RelateAtomTypeDB::RelateAtomTypeDB(const RelateAtomTypeDB &other)
                 : RelationshipDB(other), relatemrdb(0), namedb(0)
{}

/** Destructor */
RelateAtomTypeDB::~RelateAtomTypeDB()
{}

/** Initialise the database - get the references to NameDB and RelateMRDB correctly */
void RelateAtomTypeDB::initialise()
{
    RelationshipDB::initialise();

    //this component requires NameDB and RelateMRDB
    parent().addComponent<SireDB::NameDB>();
    parent().addComponent<SireDB::RelateMRDB>();

    //get pointers to these components
    namedb = &(parent().asA<NameDB>());
    relatemrdb = &(parent().asA<RelateMRDB>());
}

/** Prepare the database for dumping */
void RelateAtomTypeDB::prepareToDump()
{
    RelationshipDB::prepareToDump();

    saveParameter<RelateAtomTypeDB>("version", 1);
    saveParameter<RelateAtomTypeDB>("relatetables", QVariant::fromValue(relatetables));
}

/** Code to run after the database has been loaded */
void RelateAtomTypeDB::postLoad()
{
    RelationshipDB::postLoad();

    int v = loadParameter<RelateAtomTypeDB>("version").toInt();

    if (v == 1)
    {
        relatetables = loadParameter<RelateAtomTypeDB>("relatetables")
                                              .value< QSet<quint32> >();
    }
    else
        throw version_error( v, "1", "SireDB::RelateAtomTypeDB", CODELOC );
}

/** Tell the database to search for atom type strings using a case-sensitive
    search (this is the default - e.g. for the GAFF forcefield) */
void RelateAtomTypeDB::setCaseSensitiveAtomTypes()
{
    namedb->setCaseSensitive( atomTypes );
}

/** Tell the database to search for atom type strings using a case-insensitive
    search */
void RelateAtomTypeDB::setCaseInsensitiveAtomTypes()
{
    namedb->setCaseInsensitive( atomTypes );
}

/** Return whether or not atom types are searched for using a case-sensitive
    search */
bool RelateAtomTypeDB::caseSensitiveAtomTypes()
{
    return namedb->caseSensitive( atomTypes );
}

/** Add the new atom type string 'atomtype' and return it unique ID number.
    If this atom type string is already in the database then the ID number
    of that string is returned. */
NameID RelateAtomTypeDB::addAtomType(const QString &atomtype)
{
    return namedb->addName( atomTypes, atomtype );
}

/** Return the unique ID number of the atom type string 'atomtype', or 0
    if this string is not in the database */
NameID RelateAtomTypeDB::getAtomTypeID(const QString &atomtype)
{
    return namedb->getNameID( atomTypes, atomtype );
}

/** Return the atom type string with name ID 'id', or return an empty
    string if this is an unknown id number or if id == 0 */
QString RelateAtomTypeDB::getAtomType(NameID id)
{
    return namedb->getName( atomTypes, id );
}

/** Function used to returns the name of the table containing atom type relationships
    for 'n' atoms */
QString RelateAtomTypeDB::relateTable(uint n)
{
    return QString("SireDB_RelateAtomTypeDB_relate_%1").arg(n);
}

/** Create the table providing the relationships for groups of 'n' atoms
    (e.g. a bond is a group of 2 atoms). */
void RelateAtomTypeDB::createRelateTable(uint n)
{
    if (n > 0 and not relatetables.contains(n))
    {
        QStringList columns;

        for (uint i=0; i<n; ++i)
            columns.append( QString("MolResID%1 INTEGER, AtomType%1 INTEGER, "
                                    "Element%1 INTEGER").arg(i) );

        QString error = executeSQL( QString("create table '%1' ( %2, "
                            "RelateID INTEGER, Score INTEGER, PRIMARY KEY(RelateID) )")
                            .arg(relateTable(n), columns.join(", ")) );

          if (not error.isNull())
              throw SireDB::db_error(error,CODELOC);

        relatetables.insert(n);
    }
}

/** Convert the atom type matching criteria (with molecule and residue matching
    criteria) into a format that is usable by this database */
RelateAtomTypeData RelateAtomTypeDB::convert(const tuple<MatchMR, MatchAtomType> &match)
{
    //get the relationship ID and score of the molecule and residue matching
    //criteria
    tuple<RelateID,int> idscore = relatemrdb->get( match.get<0>() );

    //get the matched atom type
    const AtomType &atomtype = match.get<1>().type();

    //get the ID number for the atom type name identification string
    NameID atomid = addAtomType( atomtype.ID() );

    return RelateAtomTypeData( idscore, atomid, atomtype.element().nProtons() );
}

/** Convert the atom type matching criteria into a format that is usable
    by this database */
RelateAtomTypeData RelateAtomTypeDB::convert(const MatchAtomType &match)
{
    const AtomType &atomtype = match.type();

    NameID atomid = addAtomType( atomtype.ID() );

    return RelateAtomTypeData( tuple<RelateID,int>(0,0), atomid,
                               atomtype.element().nProtons() );
}

/** Convert the atom type matching criteria for the group of n atoms into
    a format that is usable by the database */
RelateAtomTypeDataList RelateAtomTypeDB::convert(const MatchAtomTypeData &match, uint n)
{
    RelateAtomTypeDataList relatedata;

    for (uint i=0; i<n; ++i)
        relatedata.append( convert( match.getMatch(i) ) );

    return relatedata;
}

/** Convert the matching criteria in 'relatedata' back into a user-friendly format. */
tuple<MatchMR,MatchAtomType> RelateAtomTypeDB::convert(const RelateAtomTypeData &relatedata)
{
    //get the molecule and residue matching criteria...
    MatchMR matchmr = relatemrdb->get( relatedata.molResID() );

    //now get the atom type identification string
    QString atomid = this->getAtomType( relatedata.atomType() );

    //return the match
    return tuple<MatchMR,MatchAtomType>( matchmr,
                      MatchAtomType( atomid, Element(relatedata.nprotons()) ) );
}

/** Convert the matching criteria in 'relatedata' back into a user-friendly format. */
MatchAtomTypeData RelateAtomTypeDB::convert(const RelateAtomTypeDataList &relatedata)
{
    MatchAtomTypeData ret;

    uint n = relatedata.count();
    for (uint i=0; i<n; ++i)
    {
        ret.set( i, convert(relatedata.at(i)) );
    }

    return ret;
}

/** Get the RelateID of the matching criteria in 'relatedata' */
RelateID RelateAtomTypeDB::get(const RelateAtomTypeDataList &relatedata)
{
    uint n = relatedata.count();
    if (not relatetables.contains(n) )
        createRelateTable(n);

    QStringList querystrings;
    for (uint i=0; i<n; ++i)
        querystrings.append( relatedata.at(i).toQueryString(i) );

    QSqlQuery q(database());
    q.exec( QString("select RelateID from '%1' where ( %2 )")
                .arg( relateTable(n), querystrings.join(" and ") ) );
    checkErrors(q, CODELOC);

    q.next();
    if (q.isValid())
        //we have found the relationship!
        return q.value(0).toUInt();
    else
        //there is no such relationship in the database
        return 0;
}

/** Add the matching criteria 'match' involving a group of 'n' atoms
    and return its unique relationship ID number - if this set of matching
    criteria is already in the database then the ID number of the existing
    entry is returned. */
RelateID RelateAtomTypeDB::add(const MatchAtomTypeData &match, uint n)
{
    if (n == 0 or match.isEmpty())
        return 0;
    else
    {
        //convert the match into a format that may be understood by
        //the database
        RelateAtomTypeDataList relatedata = convert(match,n);

        return add(relatedata);
    }
}

/** Add the matching criteria 'match' and return its unique relationship ID
    number - if this set of matching criteria is already in the database
    then the ID number of the existing entry is returned. */
RelateID RelateAtomTypeDB::add(const MatchAtomType &match)
{
    if (match.isEmpty())
        return 0;
    else
    {
        RelateAtomTypeData relatedata = convert(match);
        RelateAtomTypeDataList list;
        list.append( relatedata );

        return add(list);
    }
}

/** Private function used to add the matching criteria in 'relatedata' to the database */
RelateID RelateAtomTypeDB::add(const RelateAtomTypeDataList &relatedata)
{
    uint n = relatedata.count();

    //see if this match has already been added to the database
    RelateID relateid = get(relatedata);

    if (relateid == 0)
    {
        //there is no match - insert this into the database
        if (not relatetables.contains(n))
            createRelateTable(n);

        QString tablename = relateTable(n);

        QStringList insertstrings;
        int score = 0;

        for (uint i=0; i<n; ++i)
        {
            const RelateAtomTypeData &data = relatedata.at(i);
            insertstrings.append( data.toInsertString() );

            //increment the score
            score += data.score();
        }

        //get a new, unique relationship ID number
        relateid = this->getNewRelateID();

        QSqlQuery q(database());
        q.exec( QString("insert into '%1' values ( %2, %3, %4 )")
                  .arg( tablename, insertstrings.join(", ") )
                  .arg( relateid ).arg( score )  );
        checkErrors(q, CODELOC);
    }

    return relateid;
}

/** Return the unique relationship ID for the matching criteria in 'match'
    for the group of 'n' atoms. This returns 0 if this relationship
    is not in the database */
RelateID RelateAtomTypeDB::get(const MatchAtomTypeData &match, uint n)
{
    if (n == 0 or match.isEmpty() or not relatetables.contains(n))
        return 0;
    else
        return get( convert(match,n) );
}

/** Return the unique relationship ID for the matching criteria in 'match'.
    This returns 0 if this relationship is not in the database */
RelateID RelateAtomTypeDB::get(const MatchAtomType &match)
{
    if (match.isEmpty())
        return 0;
    else
    {
        RelateAtomTypeData relatedata = convert(match);
        RelateAtomTypeDataList list;
        list.append(relatedata);
        return get( list );
    }
}

/** Return the matching criteria that correspond to the relationship ID
    'relateid' for the group of 'n' atoms. This will return an empty
    match if there is no such relationship in this database. */
MatchAtomTypeData RelateAtomTypeDB::get(RelateID relateid, uint n)
{
    if (n == 0 or relateid == 0 or not relatetables.contains(n))
        return MatchAtomTypeData();
    else
    {
        QString tablename = relateTable(n);

        QStringList parts;
        for (uint i=0; i<n; ++i)
            parts.append( QString("MolResID%1, AtomType%1, Element%1").arg(i) );

        QSqlQuery q(database());

        q.exec( QString("select %1 from '%2' where RelateID = %3")
                  .arg(parts.join(", "), tablename).arg(relateid) );
        checkErrors(q, CODELOC);

        q.next();
        if (q.isValid())
        {
            RelateAtomTypeDataList match;

            for (uint i=0; i<n; ++i)
            {
                uint p = 3*i;

                match.append( RelateAtomTypeData(
                                q.value(p+0).toUInt(),  //MolResID%i
                                q.value(p+1).toUInt(),  //AtomType%i
                                q.value(p+2).toUInt()   //Element%i
                                                )
                            );
            }

            return convert(match);
        }
        else
        {
            //relationship was not found!
            addLog( QString("WARNING: Could not find RelateAtomTypeDB relationship with "
                            "RelateID == %1 for n == %2").arg(relateid).arg(n) );
            return MatchAtomTypeData();
        }
    }
}

/** Private function used by RelateAtomTypeDB::search to convert the RelateIDs of
    the matching molecule and residue criteria into a string suitable for
    making a database query */
QString RelateAtomTypeDB::queryString(const RelateIDMap &molresmap, uint i)
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

/** Search for all of the relationships that match 'match' for a group of 'n' atoms,
    and return thier IDs indexed by their scores. This returns an empty map if
    there are no relationships that match 'match' */
RelateIDMap RelateAtomTypeDB::findMatches(const MatchAtomTypeData &match, uint n)
{
    if (n==0 or not relatetables.contains(n))
        return RelateIDMap();
    else
    {
        QStringList querystrings;

        for (uint i=0; i<n; ++i)
        {
            tuple<MatchMR,MatchAtomType> m = match.getMatch(i);

            //search for all of the matching molecule and residue matches
            RelateIDMap molresmatch = relatemrdb->search(m.get<0>());

            //get the NameID of the atom type
            NameID atomid = this->addAtomType(m.get<1>().type().ID());

            //convert this all into a query string
            querystrings.append( QString("(AtomType%1 = %2 or AtomType%1 = 0) and "
                                         "(Element%1 = %3 or Element%1 = 0) and "
                                         "(%4)")
                                    .arg(i)
                                    .arg(atomid).arg(m.get<1>().element().nProtons())
                                    .arg(queryString(molresmatch,i)) );
        }

        //perform the search
        QSqlQuery q(database());

        q.exec( QString("select RelateID, Score from '%1' where ( %2 )")
                  .arg( relateTable(n), querystrings.join(" and ") ) );
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
RelateIDMap RelateAtomTypeDB::findMatches(const QList<AtomIndex> &atoms,
                                          const Molecule &molecule,
                                          const ParameterTable &param_table,
                                          const MatchMRData &matchmr)
{
    //create a MatchAtomTypeData for these atoms...
    int nats = atoms.count();

    QStringList atms;
    foreach(AtomIndex atm, atoms)
        atms.append( atm.toString() );

    MatchAtomTypeData matchdata;

    //get information about the molecule
    MatchMol matchmol;
    matchmol.setName( molecule.name() );

    //get a pointer to the AtomTypeTable (if there is one)
    const AtomTypeTable *typtable = 0;

    if (param_table.isA<AtomTypeTable>())
        typtable = &(param_table.asA<AtomTypeTable>());

    for (int i=0; i<nats; ++i)
    {
        //now about each atom in the group
        const AtomInfo &atom = molecule.info().atom(atoms[i]);

        MatchRes matchres;
        matchres.setName( molecule.residueName(atom.resNum()) );
        matchres.setNumber( atom.resNum() );

        //we only need to provide information about which atoms are involved in
        //bonds, as this is enough information for the database to find
        //relationships that match this bonding pattern (the database does not
        //need us to provide which atoms are not involved in bonds, as this
        //is implied from what is not in bondedAtoms())
        matchres.addResResBonds( molecule.connectivity(atom.resNum())
                                              .interBondedAtoms() );

        MatchAtomType matchtyp;

        if (typtable and typtable->assignedParameter(atom))
            //get the atom type from the one tha was assigned
            //in the parameter table
            matchtyp.setType( typtable->parameter(atom) );
        else
            //we can only set the element part of the atom type
            matchtyp.setElement( atom.element() );

        matchdata.set( i, tuple<MatchMR,MatchAtomType>( MatchMR(matchmol,matchres),
                                                        matchtyp ) );
    }

    matchdata = matchmr and matchdata;

    //search for the parameter
    return this->findMatches(matchdata, nats);
}
