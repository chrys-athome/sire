
#include "matchmol.h"

#include <QObject>
#include <QStringList>

using namespace SireDB;

/** Null constructor */
MatchMol::MatchMol()
{}

/** Copy constructor */
MatchMol::MatchMol(const MatchMol &other) 
         : molname(other.molname), molgroup(other.molgroup),
           molstate(other.molstate)
{}

/** Destructor */
MatchMol::~MatchMol()
{}

/** Comparison operator */
bool MatchMol::operator==(const MatchMol &other) const
{
    return molname == other.molname and molgroup == other.molgroup and 
           molstate == other.molstate;
}

/** Comparison operator */
bool MatchMol::operator!=(const MatchMol &other) const
{
    return not operator==(other);
}

/** Return whether this is empty (has no criteria for matching the molecule */
bool MatchMol::isEmpty() const
{
    return molname.isEmpty() and molgroup.isEmpty() and molstate.isEmpty();
}

/** Return whether this has a name match */
bool MatchMol::hasNameMatch() const
{
    return not molname.isEmpty();
}

/** Set the name criteria - the molecule's name must match 'name' */
void MatchMol::setName(const QString &name)
{
    molname = name;
}

/** Return the molecule name that must be matched to match this criteria.
    A null/empty string is returned if there are no molecule name criteria
    or if this criterium is not valid */
QString MatchMol::name() const
{
    return molname;
}

/** Return whether this has a group match */
bool MatchMol::hasGroupMatch() const
{
    return not molgroup.isEmpty();
}

/** Set the group criteria - the molecule's group must match 'group' */
void MatchMol::setGroup(const QString &group)
{
    molgroup = group;
}

/** Return the molecule group that must be matched to match this criteria.
    A null/empty string is returned if there are no molecule group criteria. */
QString MatchMol::group() const
{
    return molgroup;
}

/** Return whether this has a state match */
bool MatchMol::hasStateMatch() const
{
    return not molstate.isEmpty();
}

/** Set the state criteria - the molecule's state must match 'state' */
void MatchMol::setState(const QString &state)
{
    molstate = state;
}

/** Return the molecule state that must be matched to match this criteria.
    A null/empty string is returned if there are no molecule state criteria. */
QString MatchMol::state() const
{
    return molstate;
}

/** Return a string representation of this object */
QString MatchMol::toString() const
{
    QStringList parts;
    
    if (hasNameMatch())
        parts.append( QObject::tr("mol_name == %1").arg(molname) );
        
    if (hasGroupMatch())
        parts.append( QObject::tr("mol_group == %1").arg(molgroup) );
        
    if (hasStateMatch())
        parts.append( QObject::tr("mol_state == %1").arg(molstate) );
        
    if (parts.isEmpty())
        return "*";
    else
        return parts.join( QObject::tr(" and ") );
}

/** And-combine this molecule matching criteria with another. Note that this will
    override the first match if the second match is contradictory, e.g.
    mol_name == "p38" and mol_name == "calix" will return a match with 
    mol_name == "calix" */
MatchMol MatchMol::operator&&(const MatchMol &other) const
{
    MatchMol ret(*this);
    
    if (other.hasNameMatch())
        ret.setName( other.name() );
        
    if (other.hasGroupMatch())
        ret.setGroup( other.group() );

    if (other.hasStateMatch())
        ret.setState( other.state() );
        
    return ret;
}
