
#include "matchatomtype.h"
#include "relateatomtypedb.h"

using namespace SireDB;

/** Create a wild match */
MatchAtomType::MatchAtomType() : MatchData()
{}

/** Create a match for a specific atom type */
MatchAtomType::MatchAtomType(const AtomType &typ)
              : MatchData(), atomtype(typ)
{}

/** Create a match for a specific element */
MatchAtomType::MatchAtomType(const Element &element)
              : MatchData(), atomtype(element)
{}

/** Create a match for a specific type and element */
MatchAtomType::MatchAtomType(const QString &id, const Element &element)
              : MatchData(), atomtype(id, element)
{}

/** Copy constructor */
MatchAtomType::MatchAtomType(const MatchAtomType &other)
              : MatchData(), atomtype(other.atomtype)
{}

/** Destructor */
MatchAtomType::~MatchAtomType()
{}

/** Comparison operator */
bool MatchAtomType::operator==(const MatchAtomType &other) const
{
    return atomtype == other.atomtype;
}

/** Comparison operator */
bool MatchAtomType::operator!=(const MatchAtomType &other) const
{
    return atomtype != other.atomtype;
}

/** Return whether or not this is a wild match (matches everything) */
bool MatchAtomType::isWild() const
{
    return atomtype.isWild();
}
    
/** Synonym for isWild() */
bool MatchAtomType::isEmpty() const
{
    return atomtype.isWild();
}
    
/** Return whether this is a wild type match (matches any type, but a
    specific element, e.g. all carbons) */
bool MatchAtomType::isWildType() const
{
    return atomtype.isWildType();
}

/** Return whether this is a wild element match (matches a specific
    type, but any element of that type, e.g. all "CH2") */
bool MatchAtomType::isWildElement() const
{
    return atomtype.isWildElement();
}

/** Return a string representation of this match */
QString MatchAtomType::toString() const
{
    if (this->isWild())
        return "*";
    else if (this->isWildType())
        return QObject::tr("atom_type == %1").arg(atomtype.ID());
    else if (this->isWildElement())
        return QObject::tr("atom_element == %1").arg(atomtype.element().name());
    else
        return QObject::tr("atom_type == %1").arg(atomtype.toString());
}

/** Return a string representation for the nth atom match */
QString MatchAtomType::toString(uint n) const
{
    if (this->isWild())
        return "*";
    else if (this->isWildType())
        return QObject::tr("atom%1_type == %2").arg(n).arg(atomtype.ID());
    else if (this->isWildElement())
        return QObject::tr("atom%1_element == %2").arg(n).arg(atomtype.element().name());
    else
        return QObject::tr("atom%1_type == %2").arg(n).arg(atomtype.toString());
}
    
/** Set the type of the match (doesn't affect the element) */
void MatchAtomType::setType(const QString &atype)
{
    atomtype = AtomType(atype, atomtype.element());
}

/** Set the type of the match */
void MatchAtomType::setType(const AtomType &atype)
{
    atomtype = atype;
}
    
/** Return the atomtype that is matched */
const AtomType& MatchAtomType::type() const
{
    return atomtype;
}
    
/** Set the element to be matched */
void MatchAtomType::setElement(const Element &element)
{
    atomtype = AtomType(atomtype.ID(), element);
}

/** Return the element that is matched */
const Element& MatchAtomType::element() const
{
    return atomtype.element();
}
    
/** Combine (and) two MatchAtomType objects together. The data in the 
    second object is used if there is a contradiction, e.g. 
    atom_type == AtomType("CA", "Carbon") and atom_type == "CT"
    would return a MatchAtomType with AtomType("CT", "Carbon") */
MatchAtomType MatchAtomType::operator&&(const MatchAtomType &other) const
{
    QString id = atomtype.ID();
    Element elmnt = atomtype.element();
    
    if (not other.isWildType())
        id = other.type().ID();
        
    if (not other.isWildElement())
        elmnt= other.element();

    return MatchAtomType(id, elmnt);
}

/** This only matches a single atom */
int MatchAtomType::nMatches() const
{
    return 1;
}

/** This can only be added to match a single atom */
RelateID MatchAtomType::addTo(ParameterDB &db, uint n) const
{
    //can only match a single atom
    if (n != 1 or this->isEmpty())
        return 0;
    else
    {
        //ensure that the database has the right component(parse_0)
        db.addComponent<RelateAtomTypeDB>();
        
        return db.asA<RelateAtomTypeDB>().add(*this);
    }
}

/** This can only be added to match a single atom */
RelateID MatchAtomType::addMirroredTo(ParameterDB &db, uint n) const
{
    return this->addTo(db,n);
}
