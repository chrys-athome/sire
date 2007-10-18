/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOVE_VOLCHANGINGFUNCTION_H
#define SIREMOVE_VOLCHANGINGFUNCTION_H

#include "SireBase/sharedpolypointer.hpp"

#include "SireMaths/rangenerator.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VolChangingFunctionBase;
class VolChangingFunction;

class UniformVolumeChange;
}

QDataStream& operator<<(QDataStream&, const SireMove::VolChangingFunctionBase&);
QDataStream& operator>>(QDataStream&, SireMove::VolChangingFunctionBase&);

QDataStream& operator<<(QDataStream&, const SireMove::VolChangingFunction&);
QDataStream& operator>>(QDataStream&, SireMove::VolChangingFunction&);

QDataStream& operator<<(QDataStream&, const SireMove::UniformVolumeChange&);
QDataStream& operator>>(QDataStream&, SireMove::UniformVolumeChange&);

namespace SireVol
{
class Space;
}

namespace SireMove
{

using SireVol::Space;
using SireMaths::RanGenerator;

/** This is the base class of all functions that change the volume of
    a Space during a Monte Carlo move

    @author Christopher Woods
*/
class SIREMOVE_EXPORT VolChangingFunctionBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const VolChangingFunctionBase&);
friend QDataStream& ::operator>>(QDataStream&, VolChangingFunctionBase&);

public:
    VolChangingFunctionBase();

    VolChangingFunctionBase(const VolChangingFunctionBase &other);

    virtual ~VolChangingFunctionBase();

    static const char* typeName()
    {
        return "SireMove::VolChangingFunctionBase";
    }

    virtual const char* what() const=0;

    virtual VolChangingFunctionBase* clone() const=0;

    virtual Space change(const Space &old_space, RanGenerator generator) const=0;

    virtual void assertCompatibleWith(const Space &space) const=0;
};

/** This function changes a space uniformly by a set volume - this scales each
    of the three dimensions of the space uniformly.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT UniformVolumeChange : public VolChangingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const UniformVolumeChange&);
friend QDataStream& ::operator>>(QDataStream&, UniformVolumeChange&);

public:
    UniformVolumeChange();
    UniformVolumeChange(SireUnits::Dimension::Volume delta);

    UniformVolumeChange(const UniformVolumeChange &other);

    ~UniformVolumeChange();

    static const char* typeName()
    {
        return "SireMove::UniformVolumeChange";
    }

    const char* what() const
    {
        return UniformVolumeChange::typeName();
    }

    UniformVolumeChange* clone() const
    {
        return new UniformVolumeChange(*this);
    }

    SireUnits::Dimension::Volume maximumVolumeChange() const;

    Space change(const Space &old_space,
                 RanGenerator generator = RanGenerator()) const;

    void assertCompatibleWith(const Space &space) const;
    
private:
    #ifndef SKIP_BROKEN_GCCXML_PARTS
    /** The maximum amount by which the volume can
        change in each move */
    SireUnits::Dimension::Volume maxdelta;
    #endif
};

/** This is the holder for all volume changing functions

    @author Christopher Woods
*/
class SIREMOVE_EXPORT VolChangingFunction
{

friend QDataStream& ::operator<<(QDataStream&, const VolChangingFunction&);
friend QDataStream& ::operator>>(QDataStream&, VolChangingFunction&);

public:
    VolChangingFunction();
    VolChangingFunction(const VolChangingFunctionBase &changer);

    VolChangingFunction(const VolChangingFunction &other);

    ~VolChangingFunction();

    VolChangingFunction& operator=(const VolChangingFunctionBase &changer);
    VolChangingFunction& operator=(const VolChangingFunction &other);

    const char* what() const;

    const VolChangingFunctionBase& base() const;

    Space change(const Space &old_space,
                 RanGenerator generator = RanGenerator()) const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

    void assertCompatibleWith(const Space &space) const;

private:
    /** Implicitly shared pointer to the volume changing function */
    SireBase::SharedPolyPointer<VolChangingFunctionBase> d;
};

}

Q_DECLARE_METATYPE(SireMove::UniformVolumeChange);
Q_DECLARE_METATYPE(SireMove::VolChangingFunction);

SIRE_END_HEADER

#endif
