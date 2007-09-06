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

#ifndef SIREMOL_WEIGHTFUNCTION_H
#define SIREMOL_WEIGHTFUNCTION_H

#include <QVarLengthArray>
#include <QHash>
#include <QString>
#include <QVector>

#include <boost/any.hpp>

#include "atom.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** The number of atoms stored on the stack in WeightFunctionAtoms */
const int MaxWeightFunctionAtoms = 64;

/** This class is used as a temporary cache of atoms, while the
    atoms are being compared. This acts as a buffer, which will
    reduce the number of virtual function calls. */
typedef QVarLengthArray<Atom,MaxWeightFunctionAtoms> WeightFunctionAtoms;

class WeightFunction;

/**
This class is used to provide the temporary storage for a Weight calculation. This
allows the calculation to be performed (as much as possible) on the stack, and avoids the
need for the polymorphic WeightFunction class to be held as a pointer.

\author Christopher Woods
*/
class SIREMOL_EXPORT WeightCalculator
{

friend class WeightFunction;

public:
    WeightCalculator(const WeightFunction &weightfunction);
    ~WeightCalculator();

    void addToA(const Atom &atom);
    void addToB(const Atom &atom);

    void addToA(const QVector<Atom> &atoms);
    void addToB(const QVector<Atom> &atoms);

    double weight() const;

protected:

    /** These functions are the only ones that the 'WeightFunction' should
        call. Please respect the privacy of this class and do not access
        any private data. */

    void setValue(const QString &name, const boost::any &value);

    boost::any getValue(const QString &name, const boost::any &devault) const;

    const WeightFunctionAtoms& groupA() const;
    const WeightFunctionAtoms& groupB() const;

    quint32 nAtomsA() const;
    quint32 nAtomsB() const;

private:
    void processBuffers();

    /** Const reference to the actual WeightFunction used to
        perform the calculation. */
    const WeightFunction &weightfunc;

    /** Temporary buffer that holds the atoms in groupa as they
        are being added */
    WeightFunctionAtoms group_a;

    /** Temporary buffer that holds the atoms in groupb as they
        are being added */
    WeightFunctionAtoms group_b;

    /** The hash of any additional data required by the weight function,
        indexed by name. */
    QHash<QString, boost::any> vals;

    /** The total number of atoms that have been added to group A */
    quint32 natms_a;

    /** The total number of atoms that have been added to group B */
    quint32 natms_b;
};

/** Set the current value of the parameter named 'name'. Note that this will
    overwrite any pre-existing value. */
inline void WeightCalculator::setValue(const QString &name, const boost::any &value)
{
    vals.insert(name, value);
}

/** Return the current value of the parameter named 'name', or return 'devault' if this
    parameter does not exist. */
inline boost::any WeightCalculator::getValue(const QString &name,
                                             const boost::any &devault) const
{
    return vals.value(name, devault);
}

/** Return the buffer of atoms in group A. */
inline const WeightFunctionAtoms& WeightCalculator::groupA() const
{
    return group_a;
}

/** Return the buffer of atoms in group B. */
inline const WeightFunctionAtoms& WeightCalculator::groupB() const
{
    return group_b;
}

/** Return the total number of atoms that have been added to group A (including
    those currently in the buffer) */
inline quint32 WeightCalculator::nAtomsA() const
{
    return natms_a;
}

/** Return the total number of atoms that have been added to group B (including
    those currently in the buffer) */
inline quint32 WeightCalculator::nAtomsB() const
{
    return natms_b;
}


/**
This is the base class of all weight functions. A weight function is used to calculate the relative weight of two groups of atoms. This is used primarily in the internal move code, whereby the amount of the move is shared between the two moving halves based on their relative weights.

This is a completely const class that may not possess any member data. It uses the non-polymorphic WeightCalculator class to actually store the intermediate results of the calculation.

@author Christopher Woods
*/
class SIREMOL_EXPORT WeightFunction
{
public:
    WeightFunction()
    {}

    virtual ~WeightFunction()
    {}

    virtual const char* what() const=0;

    virtual double calculateWeight(const WeightCalculator &calculator) const=0;
    virtual void processBuffers(WeightCalculator &calculator) const=0;

protected:
    void setValue(WeightCalculator &calc, const QString &name, const boost::any &value) const;

    boost::any getValue(const WeightCalculator &calc, const QString &name,
                        const boost::any &devault) const;

    const WeightFunctionAtoms& groupA(const WeightCalculator &calc) const;
    const WeightFunctionAtoms& groupB(const WeightCalculator &calcs) const;

    quint32 nAtomsA(const WeightCalculator &calc) const;
    quint32 nAtomsB(const WeightCalculator &calc) const;
};

/** Convienience function, calling the equivalent on WeightCalculator */
inline void WeightFunction::setValue(WeightCalculator &calc, const QString &name,
                                     const boost::any &value) const
{
    calc.setValue(name, value);
}

/** Convienience function, calling the equivalent on WeightCalculator */
inline boost::any WeightFunction::getValue(const WeightCalculator &calc, const QString &name,
                                           const boost::any &devault) const
{
    return calc.getValue(name, devault);
}

/** Convienience function, calling the equivalent on WeightCalculator */
inline const WeightFunctionAtoms& WeightFunction::groupA(const WeightCalculator &calc) const
{
    return calc.groupA();
}

/** Convienience function, calling the equivalent on WeightCalculator */
inline const WeightFunctionAtoms& WeightFunction::groupB(const WeightCalculator &calc) const
{
    return calc.groupB();
}

/** Convienience function, calling the equivalent on WeightCalculator */
inline quint32 WeightFunction::nAtomsA(const WeightCalculator &calc) const
{
    return calc.nAtomsA();
}

/** Convienience function, calling the equivalent on WeightCalculator */
inline quint32 WeightFunction::nAtomsB(const WeightCalculator &calc) const
{
    return calc.nAtomsB();
}


/** This class calculates the weight by assigning all of the weight to
    the group with the largest number of atoms.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AbsFromNumber : public WeightFunction
{
public:
    AbsFromNumber() : WeightFunction()
    {}

    ~AbsFromNumber()
    {}

    const char* what() const
    {
        return "SireMol::AbsFromNumber";
    }

    void processBuffers(WeightCalculator &calculator) const;
    double calculateWeight(const WeightCalculator &calculator) const;
};

/** This class calculates the weight by assigning the weight based on the
    ratio of the number of atoms in the two groups.

    @author Christopher Woods
*/
class SIREMOL_EXPORT RelFromNumber : public AbsFromNumber
{
public:
    RelFromNumber() : AbsFromNumber()
    {}

    ~RelFromNumber()
    {}

    const char* what() const
    {
        return "SireMol::RelFromNumber";
    }

    double calculateWeight(const WeightCalculator &calculator) const;
};

/** This class calculates the weight by assigning all of the weight to
    the heaviest group (greatest mass)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AbsFromMass : public WeightFunction
{
public:
    AbsFromMass() : WeightFunction()
    {}

    ~AbsFromMass()
    {}

    const char* what() const
    {
        return "SireMol::AbsFromMass";
    }

    void processBuffers(WeightCalculator &calculator) const;
    double calculateWeight(const WeightCalculator &calculator) const;
};

/** This class calculates the weight by assigning the weight based on the
    ratio of the masses of the two groups.

    @author Christopher Woods
*/
class SIREMOL_EXPORT RelFromMass : public AbsFromMass
{
public:
    RelFromMass() : AbsFromMass()
    {}

    ~RelFromMass()
    {}

    const char* what() const
    {
        return "SireMol::RelFromMass";
    }

    double calculateWeight(const WeightCalculator &calculator) const;
};

}

SIRE_END_HEADER

#endif
