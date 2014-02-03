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

#ifndef SIREMM_CLJFUNCTION_H
#define SIREMM_CLJFUNCTION_H

#include "cljatoms.h"

#include "SireBase/property.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJFunction;
class CLJVacShiftAriFunction;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJVacShiftAriFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJVacShiftAriFunction&);

namespace SireVol
{
class Space;
}

namespace SireMM
{

using SireUnits::Dimension::Length;

using SireVol::Space;

/** Base class of all CLJFunctions. These are function classes that
    calculate the coulomb and LJ energy of the passed CLJAtoms groups
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJFunction : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFunction &func);
friend QDataStream& ::operator>>(QDataStream&, CLJFunction &func);

public:
    CLJFunction();
    CLJFunction(const CLJFunction &other);
    
    virtual ~CLJFunction();
    
    void operator()(const CLJAtoms &atoms,
                    double &cnrg, double &ljnrg) const;
    
    void operator()(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                    double &cnrg, double &ljnrg) const;

    void total(const CLJAtoms &atoms,
               double &cnrg, double &ljnrg) const;
    
    void total(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
               double &cnrg, double &ljnrg) const;
    
    double coulomb(const CLJAtoms &atoms) const;
    double coulomb(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;
    
    double lj(const CLJAtoms &atoms) const;
    double lj(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;

    virtual CLJFunction* clone() const=0;

    virtual bool hasCutoff() const;
    
    virtual Length coulombCutoff() const;
    virtual Length ljCutoff() const;

    virtual const Space& space() const;

    void setArithmeticCombiningRules(bool on);
    void setGeometricCombiningRules(bool on);
    
    bool usingArithmeticCombiningRules() const;
    bool usingGeometricCombiningRules() const;

protected:
    CLJFunction& operator=(const CLJFunction &other);
    
    bool operator==(const CLJFunction &other) const;

    virtual void calcEnergyAri(const CLJAtoms &atoms,
                               double &cnrg, double &ljnrg) const=0;

    virtual void calcEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                               double &cnrg, double &ljnrg) const=0;

    virtual void calcEnergyGeo(const CLJAtoms &atoms,
                               double &cnrg, double &ljnrg) const=0;

    virtual void calcEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                               double &cnrg, double &ljnrg) const=0;

    virtual double calcCoulombEnergyAri(const CLJAtoms &atoms) const;
    virtual double calcCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;

    virtual double calcCoulombEnergyGeo(const CLJAtoms &atoms) const;
    virtual double calcCoulombEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;
    
    virtual double calcLJEnergyAri(const CLJAtoms &atoms) const;
    virtual double calcLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;
    
    virtual double calcLJEnergyGeo(const CLJAtoms &atoms) const;
    virtual double calcLJEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;

private:
    /** whether or not to use arithmetic combining rules */
    bool use_arithmetic;
};

/** This CLJFunction calculates the coulomb and LJ energy of the passed
    CLJAtoms using a shift-electrostatics cutoff and an infinite box,
    with arithmetic combining rules used for the LJ combining rules
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJVacShiftAriFunction
        : public SireBase::ConcreteProperty<CLJVacShiftAriFunction,CLJFunction>
{

friend QDataStream& ::operator<<(QDataStream&, const CLJVacShiftAriFunction&);
friend QDataStream& ::operator>>(QDataStream&, CLJVacShiftAriFunction&);

public:
    CLJVacShiftAriFunction();
    CLJVacShiftAriFunction(Length cutoff);
    CLJVacShiftAriFunction(Length coul_cutoff, Length lj_cutoff);
    
    CLJVacShiftAriFunction(const CLJVacShiftAriFunction &other);
    
    ~CLJVacShiftAriFunction();
    
    CLJVacShiftAriFunction& operator=(const CLJVacShiftAriFunction &other);
    
    bool operator==(const CLJVacShiftAriFunction &other) const;
    bool operator!=(const CLJVacShiftAriFunction &other) const;
    
    static const char* typeName();
    const char* what() const;
    
    CLJVacShiftAriFunction* clone() const;

    bool hasCutoff() const;
    
    Length coulombCutoff() const;
    Length ljCutoff() const;

protected:
    void calcEnergyAri(const CLJAtoms &atoms,
                       double &cnrg, double &ljnrg) const;
    
    void calcEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                       double &cnrg, double &ljnrg) const;

    void calcEnergyGeo(const CLJAtoms &atoms,
                       double &cnrg, double &ljnrg) const;
    
    void calcEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                       double &cnrg, double &ljnrg) const;
    
private:
    /** The coulomb cutoff */
    float coul_cutoff;
    
    /** The LJ cutoff */
    float lj_cutoff;
};

}

Q_DECLARE_METATYPE( SireMM::CLJVacShiftAriFunction )

SIRE_EXPOSE_CLASS( SireMM::CLJFunction )

SIRE_EXPOSE_CLASS( SireMM::CLJVacShiftAriFunction )

/*
SIRE _EXPOSE_CLASS( SireMM::CLJVacShiftGeoFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJVacRFAriFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJVacRFGeoFunction )

SIRE _EXPOSE_CLASS( SireMM::CLJVacAriFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJVacGeoFunction )

SIRE _EXPOSE_CLASS( SireMM::CLJBoxShiftAriFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJBoxShiftGeoFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJBoxRFAriFunction )
SIRE _EXPOSE_CLASS( SireMM::CLJBoxRFGeoFunction )
*/

SIRE_END_HEADER

#endif
