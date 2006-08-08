
#include "matchatom.h"
#include "matchmra.h"

using namespace SireDB;

/** Null constructor */
MatchAtom::MatchAtom() : MatchData(), atmnum(0)
{}

/** Copy constructor */
MatchAtom::MatchAtom(const MatchAtom &other)
          : MatchData(), atmname(other.atmname), atmnum(other.atmnum)
{}

/** Destructor */
MatchAtom::~MatchAtom()
{}

/** Comparison operator */
bool MatchAtom::operator==(const MatchAtom &other) const
{
    return atmname == other.atmname and atmnum == other.atmnum;
}

/** Comparison operator */
bool MatchAtom::operator!=(const MatchAtom &other) const
{
    return atmnum != other.atmnum or atmname != other.atmname;
}

/** Return a string representation of these criteria */
QString MatchAtom::toString() const
{
    QStringList parts;
    
    if (hasNameMatch())
        parts.append( QObject::tr("atom_name == %1").arg(name()) );
        
    if (hasNumberMatch())
        parts.append( QObject::tr("atom_number == %1").arg(number().toString()) );
        
    if (parts.isEmpty())
        return QObject::tr("atom == *");
    else
        return parts.join( QObject::tr(" and ") );
}

/** Return a string representation of these criteria, using the id number 'atm' */
QString MatchAtom::toString(uint atm) const
{
    QStringList parts;
    
    if (hasNameMatch())
        parts.append( QObject::tr("atom%1_name == %2").arg(atm).arg(name()) );
        
    if (hasNumberMatch())
        parts.append( QObject::tr("atom%1_number == %2").arg(atm).arg(number().toString()) );
        
    if (parts.isEmpty())
        return QObject::tr("atom%1 == *").arg(atm);
    else
        return parts.join( QObject::tr(" and ") );
}

/** Is this an empty match (matches everything) */
bool MatchAtom::isEmpty() const
{
    return atmname.isEmpty() and atmnum == 0;
}

/** Do these criteria specify the atom's number? */
bool MatchAtom::hasNumberMatch() const
{
    return atmnum != 0;
}

/** Do these criteria specify the atom's name? */
bool MatchAtom::hasNameMatch() const
{
    return not atmname.isEmpty();
}

/** Set the name of the atom that must be matched */
void MatchAtom::setName(const QString &name)
{
    atmname = name;
}

/** Return the name of the atom that must be matched - this returns
    an empty string if there is no name requirement */
QString MatchAtom::name() const
{
    return atmname;
}

/** Set the number of the atom that must be matched */
void MatchAtom::setNumber(AtomNum number)
{
    atmnum = number;
}

/** Return the number of the atom that must be matched - this returns
    zero if there is no number requirement */
AtomNum MatchAtom::number() const
{
    return atmnum;
}

/** and-combine two sets of atom criteria together. Note that this will use
    the second match if there are any contradictions, e.g.
    atom_num == 15 and atom_num == 10 will return a MatchAtom with 
    atom_num == 10 */
MatchAtom MatchAtom::operator&&(const MatchAtom &other) const
{
    MatchAtom ret(*this);
    
    if (other.hasNameMatch())
        ret.setName( other.name() );
        
    if (other.hasNumberMatch())
        ret.setNumber( other.number() );
        
    return ret;
}

/** A single MatchAtom only matches 1 atom */
int MatchAtom::nMatches() const
{
    return 1;
}

/** Add this to the database 'db' for matching 'n' atoms, and return 
    the ID number of this matching criteria */
RelateID MatchAtom::addTo(ParameterDB &db, uint n) const
{
    return MatchMRA(*this).addTo(db,n);
}

/** Add this to the database 'db' for matching 'n' atoms, and return 
    the ID number of this matching criteria */
RelateID MatchAtom::addMirroredTo(ParameterDB &db, uint n) const
{
    return MatchMRA(*this).addMirroredTo(db,n);
}
