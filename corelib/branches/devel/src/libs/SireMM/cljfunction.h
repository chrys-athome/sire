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
#include "cljnbpairs.h"

#include "SireMol/atomidx.h"

#include "SireVol/space.h"

#include "SireBase/property.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJFunction;
class CLJCutoffFunction;
class CLJIntraFunction;
class CLJSoftFunction;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJCutoffFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJCutoffFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJIntraFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJIntraFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJSoftFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CLJSoftFunction&);

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
    enum COMBINING_RULES
    {
        ARITHMETIC = 1,
        GEOMETRIC = 2
    };

    CLJFunction();
    CLJFunction(COMBINING_RULES combining_rules);
    
    CLJFunction(const Space &space);
    CLJFunction(const Space &space, COMBINING_RULES combining_rules);
        
    CLJFunction(const CLJFunction &other);
    
    virtual ~CLJFunction();
 
    static const char* typeName();
    
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

    virtual void setCutoff(Length distance);
    virtual void setCutoff(Length coulomb_cutoff, Length lj_cutoff);

    virtual void setCoulombCutoff(Length distance);
    virtual void setLJCutoff(Length distance);

    bool isPeriodic() const;
    virtual const Space& space() const;

    virtual void setSpace(const Space &space);
    
    virtual bool isSoftened() const;

    void setArithmeticCombiningRules(bool on);
    void setGeometricCombiningRules(bool on);
    
    COMBINING_RULES combiningRules() const;
    void setCombiningRules(COMBINING_RULES rules);
    
    bool usingArithmeticCombiningRules() const;
    bool usingGeometricCombiningRules() const;

protected:
    CLJFunction& operator=(const CLJFunction &other);
    
    bool operator==(const CLJFunction &other) const;

    virtual void calcVacEnergyAri(const CLJAtoms &atoms,
                                  double &cnrg, double &ljnrg) const=0;

    virtual void calcVacEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                  double &cnrg, double &ljnrg) const=0;

    virtual void calcVacEnergyGeo(const CLJAtoms &atoms,
                                  double &cnrg, double &ljnrg) const=0;

    virtual void calcVacEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                  double &cnrg, double &ljnrg) const=0;

    virtual double calcVacCoulombEnergyAri(const CLJAtoms &atoms) const;
    virtual double calcVacCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;

    virtual double calcVacCoulombEnergyGeo(const CLJAtoms &atoms) const;
    virtual double calcVacCoulombEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;
    
    virtual double calcVacLJEnergyAri(const CLJAtoms &atoms) const;
    virtual double calcVacLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;
    
    virtual double calcVacLJEnergyGeo(const CLJAtoms &atoms) const;
    virtual double calcVacLJEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const;

    virtual void calcBoxEnergyAri(const CLJAtoms &atoms, const Vector &box,
                                  double &cnrg, double &ljnrg) const=0;

    virtual void calcBoxEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                  const Vector &box, double &cnrg, double &ljnrg) const=0;

    virtual void calcBoxEnergyGeo(const CLJAtoms &atoms, const Vector &box,
                                  double &cnrg, double &ljnrg) const=0;

    virtual void calcBoxEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                  const Vector &box, double &cnrg, double &ljnrg) const=0;

    virtual double calcBoxCoulombEnergyAri(const CLJAtoms &atoms, const Vector &box) const;
    virtual double calcBoxCoulombEnergyAri(const CLJAtoms &atoms0,
                                           const CLJAtoms &atoms1,
                                           const Vector &box) const;

    virtual double calcBoxCoulombEnergyGeo(const CLJAtoms &atoms, const Vector &box) const;
    virtual double calcBoxCoulombEnergyGeo(const CLJAtoms &atoms0,
                                           const CLJAtoms &atoms1,
                                           const Vector &box) const;
    
    virtual double calcBoxLJEnergyAri(const CLJAtoms &atoms, const Vector &box) const;
    virtual double calcBoxLJEnergyAri(const CLJAtoms &atoms0,
                                      const CLJAtoms &atoms1,
                                      const Vector &box) const;
    
    virtual double calcBoxLJEnergyGeo(const CLJAtoms &atoms, const Vector &box) const;
    virtual double calcBoxLJEnergyGeo(const CLJAtoms &atoms0,
                                      const CLJAtoms &atoms1,
                                      const Vector &box) const;

private:
    void extractDetailsFromRules(COMBINING_RULES rules);
    void extractDetailsFromSpace();

    /** The space used by the function */
    SireVol::SpacePtr spce;

    /** The dimensions of the periodic box, if used */
    Vector box_dimensions;

    /** whether or not to use arithmetic combining rules */
    bool use_arithmetic;
    
    /** Whether or not to use a periodic box */
    bool use_box;
};

/** This is the base class of all CLJ functions that have a cutoff

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJCutoffFunction : public CLJFunction
{

friend QDataStream& ::operator<<(QDataStream&, const CLJCutoffFunction&);
friend QDataStream& ::operator>>(QDataStream&, CLJCutoffFunction&);

public:
    CLJCutoffFunction();
    CLJCutoffFunction(Length cutoff);
    CLJCutoffFunction(Length coul_cutoff, Length lj_cutoff);
    
    CLJCutoffFunction(const Space &space, Length cutoff);
    CLJCutoffFunction(const Space &space, Length coul_cutoff, Length lj_cutoff);
    
    CLJCutoffFunction(Length cutoff, COMBINING_RULES combining_rules);
    CLJCutoffFunction(Length coul_cutoff, Length lj_cutoff, COMBINING_RULES combining_rules);
    
    CLJCutoffFunction(const Space &space, COMBINING_RULES combining_rules);
    CLJCutoffFunction(const Space &space, Length cutoff, COMBINING_RULES combining_rules);
    CLJCutoffFunction(const Space &space, Length coul_cutoff, Length lj_cutoff,
                      COMBINING_RULES combining_rules);
    
    CLJCutoffFunction(const CLJCutoffFunction &other);
    
    ~CLJCutoffFunction();
    
    static const char* typeName();

    bool hasCutoff() const;
    
    Length coulombCutoff() const;
    Length ljCutoff() const;
    
    void setCutoff(Length distance);
    void setCutoff(Length coulomb_cutoff, Length lj_cutoff);
    
    void setCoulombCutoff(Length distance);
    void setLJCutoff(Length distance);
    
private:
    void pvt_setCutoff( Length coulomb, Length lj );
    
protected:
    CLJCutoffFunction& operator=(const CLJCutoffFunction &other);
    
    bool operator==(const CLJCutoffFunction &other) const;
    
    /** The coulomb cutoff */
    float coul_cutoff;
    
    /** The LJ cutoff */
    float lj_cutoff;
};

/** This is the base class of all intramolecular CLJ functions

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJIntraFunction : public CLJCutoffFunction
{

friend QDataStream& ::operator<<(QDataStream&, const CLJIntraFunction&);
friend QDataStream& ::operator>>(QDataStream&, CLJIntraFunction&);

public:
    CLJIntraFunction();
    CLJIntraFunction(Length cutoff);
    CLJIntraFunction(Length coul_cutoff, Length lj_cutoff);
    
    CLJIntraFunction(const Space &space, Length cutoff);
    CLJIntraFunction(const Space &space, Length coul_cutoff, Length lj_cutoff);
    
    CLJIntraFunction(Length cutoff, COMBINING_RULES combining_rules);
    CLJIntraFunction(Length coul_cutoff, Length lj_cutoff, COMBINING_RULES combining_rules);
    
    CLJIntraFunction(const Space &space, COMBINING_RULES combining_rules);
    CLJIntraFunction(const Space &space, Length cutoff, COMBINING_RULES combining_rules);
    CLJIntraFunction(const Space &space, Length coul_cutoff, Length lj_cutoff,
                     COMBINING_RULES combining_rules);

    CLJIntraFunction(const CLJIntraFunction &other);
    
    ~CLJIntraFunction();
    
    static const char* typeName();

    void setNBPairs(const CLJNBPairs &cljnb);
    void setNBPairs(const MoleculeView &molecule, const PropertyMap &map = PropertyMap());

    const CLJNBPairs& nbPairs() const;
    
protected:
    CLJIntraFunction& operator=(const CLJIntraFunction &other);
    
    bool operator==(const CLJIntraFunction &other) const;

    QPair<MultiFloat,MultiFloat> getScaleFactors(const MultiInt &id0,
                                                 const MultiInt &id1,
                                                 bool not_bonded) const;

private:
    static qint64 getIndex(const SireMol::AtomIdx &atom0, const SireMol::AtomIdx &atom1);
    static qint64 getIndex(qint32 id0, qint32 id1);
    static qint64 pack(qint32 a, qint32 b);

    /** The CLJNBPairs from which the below scale factors are extracted */
    CLJNBPairs nbpairs;

    /** The set of 1-4 scale factors for the interatomic pairs
        in the molecule that can be evaluated by this function */
    QHash< qint64, QPair<float,float> > sclfacs;
};

/** This is the base class of all soft-core CLJ functions that have a cutoff

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJSoftFunction : public CLJCutoffFunction
{

friend QDataStream& ::operator<<(QDataStream&, const CLJSoftFunction&);
friend QDataStream& ::operator>>(QDataStream&, CLJSoftFunction&);

public:
    CLJSoftFunction();
    
    CLJSoftFunction(const CLJSoftFunction &other);
    
    ~CLJSoftFunction();

    static const char* typeName();

    bool isSoftened() const;
    
    float alpha() const;
    float shiftDelta() const;
    float coulombPower() const;
    
    void setAlpha(float alpha);
    void setShiftDelta(float shift);
    void setCoulombPower(float power);
    
private:
    void pvt_set(float alpha, float shift, float power);
    
protected:
    CLJSoftFunction& operator=(const CLJSoftFunction &other);
    
    bool operator==(const CLJSoftFunction &other) const;

    /** The value of alpha to use */
    float alpha_value;
    
    /** The value of shift-delta */
    float shift_delta;
    
    /** The value of coulomb power */
    float coulomb_power;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Private internal function that takes the passed two AtomIdx 32bit integers
    and packs them into a single 64bit integer. */
inline qint64 CLJIntraFunction::pack(qint32 a, qint32 b)
{
    qint64 ret;
    (reinterpret_cast<qint32*>(&ret))[0] = a;
    (reinterpret_cast<qint32*>(&ret))[1] = b;
    return ret;
}

/** Internal function that gets the 64bit index from the two 32bit ID numbers */
inline qint64 CLJIntraFunction::getIndex(qint32 id0, qint32 id1)
{
    return id0 <= id1 ? pack(id0,id1) : pack(id1,id0);
}

/** Internal function used to get the 64bit index into the nonbonded scale factor
    hash for the pair of atoms with AtomIdx values 'atom0' and 'atom1' */
inline qint64 CLJIntraFunction::getIndex(const SireMol::AtomIdx &atom0,
                                         const SireMol::AtomIdx &atom1)
{
    return atom0.value() <= atom1.value() ? pack(atom0.value() + 1,atom1.value() + 1) :
                                            pack(atom1.value() + 1,atom0.value() + 1);
}

/** Return the coulomb and LJ scale factors for the ID pairs in 'id0' and 'id1'.
    If 'not_bonded' is 'true', then this returns { MultiFloat(1), MultiFloat(1) } */
inline QPair<MultiFloat,MultiFloat> CLJIntraFunction::getScaleFactors(
                                                const SireMaths::MultiInt &id0,
                                                const SireMaths::MultiInt &id1,
                                                bool not_bonded) const
{
    if (not_bonded)
    {
        static const QPair<MultiFloat,MultiFloat> one =
                        QPair<MultiFloat,MultiFloat>( MultiFloat(1), MultiFloat(1) );
    
        return one;
    }
    else
    {
        MultiFloat coul(1), lj(1);
        
        for (int i=0; i<MultiFloat::count(); ++i)
        {
            const qint64 idx = getIndex(id0[0], id1[0]);
            QHash< qint64,QPair<float,float> >::const_iterator it = sclfacs.constFind(idx);
            
            if (it != sclfacs.constEnd())
            {
                coul.set(i, it.value().first);
                lj.set(i, it.value().second);
            }
        }
        
        return QPair<MultiFloat,MultiFloat>(coul,lj);
    }
}

#endif

}

SIRE_EXPOSE_CLASS( SireMM::CLJFunction )
SIRE_EXPOSE_CLASS( SireMM::CLJCutoffFunction )
SIRE_EXPOSE_CLASS( SireMM::CLJSoftFunction )
SIRE_EXPOSE_CLASS( SireMM::CLJIntraFunction )

SIRE_END_HEADER

#endif
