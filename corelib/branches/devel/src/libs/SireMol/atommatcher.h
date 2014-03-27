/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#ifndef SIREMOL_ATOMMATCHER_H
#define SIREMOL_ATOMMATCHER_H

#include <QHash>
#include <QList>
#include <QPair>

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireUnits/dimensions.h"

#include <boost/tuple/tuple.hpp>

namespace SireMol
{
class AtomMatcher;
class AtomIdxMatcher;
class AtomNameMatcher;
class AtomResultMatcher;
class AtomMatchInverter;
class AtomIDMatcher;
class AtomMultiMatcher;
class AtomMCSMatcher;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomIdxMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomIdxMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomNameMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomNameMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomIDMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomIDMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomResultMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomResultMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomMatchInverter&);
QDataStream& operator>>(QDataStream&, SireMol::AtomMatchInverter&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomMultiMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomMultiMatcher&);

QDataStream& operator<<(QDataStream&, const SireMol::AtomMCSMatcher&);
QDataStream& operator>>(QDataStream&, SireMol::AtomMCSMatcher&);

namespace SireMol
{

class AtomIdx;
class AtomName;
class AtomIdentifier;
class MoleculeView;
class MoleculeInfoData;

using SireBase::PropertyMap;

/** Virtual base class of all of the functions used to match
    atoms in one molecule layout with atoms in another layout
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomMatcher : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const AtomMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomMatcher&);

public:
    AtomMatcher();
    AtomMatcher(const AtomMatcher &other);
    
    virtual ~AtomMatcher();
    
    static const char* typeName()
    {
        return "SireMol::AtomMatcher";
    }
    
    virtual bool isNull() const;
    
    AtomMultiMatcher operator+(const AtomMatcher &other) const;
    
    AtomMultiMatcher add(const AtomMatcher &other) const;
    
    virtual AtomMatcher* clone() const=0;

    virtual bool changesOrder(const MoleculeInfoData &molinfo0,
                              const MoleculeInfoData &molinfo1) const;

    virtual bool changesOrder(const MoleculeView &molview0,
                              const MoleculeView &molview1) const;
    
    virtual bool changesOrder(const MoleculeView &molview0,
                              const PropertyMap &map0,
                              const MoleculeView &molview1,
                              const PropertyMap &map1) const;
    
    virtual bool changesOrder(const MoleculeView &molview0,
                              const MoleculeView &molview1,
                              const PropertyMap &map) const;
    
    virtual QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                         const MoleculeInfoData &molinfo1) const;
    
    virtual QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                         const PropertyMap &map0,
                                         const MoleculeView &molview1,
                                         const PropertyMap &map1) const=0;

    virtual QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                         const MoleculeView &molview1) const;
    
    virtual QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                         const MoleculeView &molview1,
                                         const PropertyMap &map) const;

    static const AtomMultiMatcher& null();
};

typedef SireBase::PropPtr<AtomMatcher> AtomMatcherPtr;

/** This is a simple atom matcher that can be used to repeat a match
    based on a prior result
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomResultMatcher
         : public SireBase::ConcreteProperty<AtomResultMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomResultMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomResultMatcher&);

public:
    AtomResultMatcher();
    AtomResultMatcher(const QHash<AtomIdx,AtomIdx> &results, bool invert=false);
    
    AtomResultMatcher(const AtomResultMatcher &other);
    
    ~AtomResultMatcher();
    
    static const char* typeName();
    
    const char* what() const
    {
        return AtomResultMatcher::typeName();
    }

    bool isNull() const;

    QString toString() const;

    AtomResultMatcher& operator=(const AtomResultMatcher &other);
    
    bool operator==(const AtomResultMatcher &other) const;
    bool operator!=(const AtomResultMatcher &other) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;

private:
    /** The result of matching using another AtomMatcher */
    QHash<AtomIdx,AtomIdx> m;
};

/** This is a atom matcher that inverts the match of the sub-matcher.
    This is useful when you want to match from molecule 1 to molecule 0
    as opposed to molecule 0 to molecule 1, and but don't want to change
    the order of the match at the calling site
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomMatchInverter
         : public SireBase::ConcreteProperty<AtomMatchInverter,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomMatchInverter&);
friend QDataStream& ::operator>>(QDataStream&, AtomMatchInverter&);

public:
    AtomMatchInverter();
    AtomMatchInverter(const AtomMatcher &matcher);
    
    AtomMatchInverter(const AtomMatchInverter &other);
    
    ~AtomMatchInverter();
    
    static const char* typeName();
    
    const char* what() const
    {
        return AtomMatchInverter::typeName();
    }

    bool isNull() const;

    QString toString() const;

    AtomMatchInverter& operator=(const AtomMatchInverter &other);
    
    bool operator==(const AtomMatchInverter &other) const;
    bool operator!=(const AtomMatchInverter &other) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;

private:
    /** The matcher used for the forwards match */
    AtomMatcherPtr m;
};

/** This is a simple atom matcher that matches the atoms based
    on their index in the molecule - e.g. it matches the first
    atom in molinfo0 to the first atom in molinfo1, the second
    atom in molinfo0 to the second atom in molinfo1, and the
    nth atom in molinfo0 to the nth atom in molinfo1
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomIdxMatcher
         : public SireBase::ConcreteProperty<AtomIdxMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomIdxMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomIdxMatcher&);

public:
    AtomIdxMatcher();
    AtomIdxMatcher(const AtomIdxMatcher&);
    
    ~AtomIdxMatcher();
    
    static const char* typeName();
    
    const char* what() const
    {
        return AtomIdxMatcher::typeName();
    }

    QString toString() const;

    AtomIdxMatcher& operator=(const AtomIdxMatcher &other);
    
    bool operator==(const AtomIdxMatcher &other) const;
    bool operator!=(const AtomIdxMatcher &other) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;
};

/** This is a simple atom matcher that matches the atoms based
    on their names, so the atom called "CA1" in molinfo0 will
    be matched to the atom called "CA1" in molinfo1
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomNameMatcher
         : public SireBase::ConcreteProperty<AtomNameMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomNameMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomNameMatcher&);

public:
    AtomNameMatcher();
    AtomNameMatcher(const AtomNameMatcher&);
    
    ~AtomNameMatcher();
    
    static const char* typeName();
    
    const char* what() const
    {
        return AtomNameMatcher::typeName();
    }

    AtomNameMatcher& operator=(const AtomNameMatcher &other);
    
    bool operator==(const AtomNameMatcher &other) const;
    bool operator!=(const AtomNameMatcher &other) const;
    
    QString toString() const;

    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;
};

/** This is an atom matcher that allows the user to specify
    exactly how one atom matches another in the molecule
    by mapping one AtomIdentifier to another
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomIDMatcher
        : public SireBase::ConcreteProperty<AtomIDMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomIDMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomIDMatcher&);

public:
    AtomIDMatcher();
    
    AtomIDMatcher(const QList< QPair<QString,QString> > &match_names);
    AtomIDMatcher(const QList< QPair<int,int> > &match_idxs);
    AtomIDMatcher(const QList< QPair<AtomIdentifier,AtomIdentifier> > &match_ids);
    
    AtomIDMatcher(const QList< boost::tuple<QString,QString> > &match_names);
    AtomIDMatcher(const QList< boost::tuple<int,int> > &match_idxs );
    AtomIDMatcher(const QList< boost::tuple<AtomIdentifier,AtomIdentifier> > &match_ids);
    
    AtomIDMatcher(const AtomIDMatcher &other);
    
    ~AtomIDMatcher();
    
    static const char* typeName();
    
    bool isNull() const;
    
    const char* what() const
    {
        return AtomIDMatcher::typeName();
    }

    AtomIDMatcher& operator=(const AtomIDMatcher &other);
    
    bool operator==(const AtomIDMatcher &other) const;
    bool operator!=(const AtomIDMatcher &other) const;
    
    QString toString() const;

    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;

private:
    /** The mapping from atom ID in molecule 0 to atom ID in molecule 1 */
    QList< QPair<AtomIdentifier,AtomIdentifier > > m;
};

/** This is an atom matcher that matches using the maximum common substructure
    of the two molecules
 
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomMCSMatcher
        : public SireBase::ConcreteProperty<AtomMCSMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomMCSMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomMCSMatcher&);

public:
    AtomMCSMatcher();
    AtomMCSMatcher(const SireUnits::Dimension::Time &timeout);
    
    AtomMCSMatcher(const AtomMCSMatcher &other);
    
    ~AtomMCSMatcher();
    
    static const char* typeName();
    
    const char* what() const
    {
        return AtomMCSMatcher::typeName();
    }

    AtomMCSMatcher& operator=(const AtomMCSMatcher &other);
    
    bool operator==(const AtomMCSMatcher &other) const;
    bool operator!=(const AtomMCSMatcher &other) const;
    
    QString toString() const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;

private:
    /** Timeout for the MCS match */
    SireUnits::Dimension::Time timeout;
};

/** This is an atom matcher combines several sub-AtomMatchers together
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomMultiMatcher
        : public SireBase::ConcreteProperty<AtomMultiMatcher,AtomMatcher>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomMultiMatcher&);
friend QDataStream& ::operator>>(QDataStream&, AtomMultiMatcher&);

public:
    AtomMultiMatcher();
    AtomMultiMatcher(const AtomMatcher &matcher);
    AtomMultiMatcher(const AtomMatcher &m0, const AtomMatcher &m1);
    
    AtomMultiMatcher(const AtomMultiMatcher &other);
    
    ~AtomMultiMatcher();
    
    static const char* typeName();
    
    bool isNull() const;
    
    const char* what() const
    {
        return AtomMultiMatcher::typeName();
    }

    AtomMultiMatcher& operator=(const AtomMultiMatcher &other);
    
    bool operator==(const AtomMultiMatcher &other) const;
    bool operator!=(const AtomMultiMatcher &other) const;
    
    QString toString() const;

    QHash<AtomIdx,AtomIdx> match(const MoleculeInfoData &molinfo0,
                                 const MoleculeInfoData &molinfo1) const;
    
    QHash<AtomIdx,AtomIdx> match(const MoleculeView &molview0,
                                 const PropertyMap &map0,
                                 const MoleculeView &molview1,
                                 const PropertyMap &map1) const;

private:
    /** The set of matches, which are processed in order */
    QList<AtomMatcherPtr> m;
};

}

Q_DECLARE_METATYPE( SireMol::AtomIdxMatcher )
Q_DECLARE_METATYPE( SireMol::AtomNameMatcher )
Q_DECLARE_METATYPE( SireMol::AtomIDMatcher )
Q_DECLARE_METATYPE( SireMol::AtomResultMatcher )
Q_DECLARE_METATYPE( SireMol::AtomMatchInverter )
Q_DECLARE_METATYPE( SireMol::AtomMultiMatcher )
Q_DECLARE_METATYPE( SireMol::AtomMCSMatcher )

SIRE_EXPOSE_CLASS( SireMol::AtomMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomIdxMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomNameMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomResultMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomMatchInverter )
SIRE_EXPOSE_CLASS( SireMol::AtomIDMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomMultiMatcher )
SIRE_EXPOSE_CLASS( SireMol::AtomMCSMatcher )

SIRE_EXPOSE_PROPERTY( SireMol::AtomMatcherPtr, SireMol::AtomMatcher )

#endif
