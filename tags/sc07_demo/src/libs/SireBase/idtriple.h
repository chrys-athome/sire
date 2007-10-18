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

#ifndef SIREBASE_IDTRIPLE_HPP
#define SIREBASE_IDTRIPLE_HPP

#include "idpair.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class IDTriple;
}

QDataStream& operator<<(QDataStream&, const SireBase::IDTriple&);
QDataStream& operator>>(QDataStream&, SireBase::IDTriple&);

namespace SireBase
{

class SIREBASE_EXPORT IDTriple
{

friend QDataStream& ::operator<<(QDataStream&, const IDTriple&);
friend QDataStream& ::operator>>(QDataStream&, IDTriple&);

private:
    static Incremint shared_triple_incremint;

public:
    IDTriple(Incremint *idint = &shared_triple_incremint);

    IDTriple(const IDTriple &other);

    IDTriple& operator=(const IDTriple &other);

    ~IDTriple();

    bool operator==(const IDTriple &other) const;
    bool operator!=(const IDTriple &other) const;

protected:
    void incrementID();
    void incrementMajor();
    void incrementMinor();

    quint32 ID() const;
    quint32 major() const;
    quint32 minor() const;

    const Version& version() const;

    bool sameMajorVersion(const IDTriple &other) const;
    bool sameMinorVersion(const IDTriple &other) const;
    bool sameVersion(const IDTriple &other) const;

    bool sameID(const IDTriple &other) const;
    bool sameIDAndVersion(const IDTriple &other) const;
    bool sameIDAndMajorVersion(const IDTriple &other) const;

private:
    /** Shared pointer to the Incremint used to increment
        the major version number */
    boost::shared_ptr<Incremint> major_incremint;

    /** Pointer to the Incremint that is used to increment
        the ID number */
    Incremint *id_incremint;

    /** The ID number */
    quint32 idnum;

    /** The major and minor numbers */
    IDPair versn;
};

/** Increment the minor version number */
inline void IDTriple::incrementMinor()
{
    versn.incrementMinor();
}

/** Increment the major version number */
inline void IDTriple::incrementMajor()
{
    versn.incrementMajor();
}

/** Increment the ID number */
inline void IDTriple::incrementID()
{
    idnum = id_incremint->increment();

    //reset the ID
    major_incremint.reset( new Incremint(0) );
    versn = IDPair(major_incremint.get());
}

/** Return the ID number */
inline quint32 IDTriple::ID() const
{
    return idnum;
}

/** Return the major number */
inline quint32 IDTriple::major() const
{
    return versn.major();
}

/** Return the minor number */
inline quint32 IDTriple::minor() const
{
    return versn.minor();
}

/** Return the version */
inline const Version& IDTriple::version() const
{
    return versn.version();
}

/** Return whether or not this has the same major version as 'other' */
inline bool IDTriple::sameMajorVersion(const IDTriple &other) const
{
    return versn.sameMajorVersion(other.versn);
}

/** Return whether or not this has the same minor version as 'other' */
inline bool IDTriple::sameMinorVersion(const IDTriple &other) const
{
    return versn.sameMinorVersion(other.versn);
}

/** Return whether or not this has the same version as 'other' */
inline bool IDTriple::sameVersion(const IDTriple &other) const
{
    return versn.sameVersion(other.versn);
}

/** Return whether or not this has the same ID as 'other' */
inline bool IDTriple::sameID(const IDTriple &other) const
{
    return idnum == other.idnum;
}

/** Return whether or not this has the same ID and version as 'other' */
inline bool IDTriple::sameIDAndVersion(const IDTriple &other) const
{
    return sameID(other) and sameVersion(other);
}

/** Return whether or not this has the same ID and major version as 'other' */
inline bool IDTriple::sameIDAndMajorVersion(const IDTriple &other) const
{
    return sameID(other) and sameMajorVersion(other);
}

/** Comparison operator */
inline bool IDTriple::operator==(const IDTriple &other) const
{
    return idnum == other.idnum and id_incremint == other.id_incremint
              and versn == other.versn;
}

/** Comparison operator */
inline bool IDTriple::operator!=(const IDTriple &other) const
{
    return idnum != other.idnum or id_incremint != other.id_incremint
              or versn != other.versn;
}

}

Q_DECLARE_METATYPE(SireBase::IDTriple);

SIRE_END_HEADER

#endif
