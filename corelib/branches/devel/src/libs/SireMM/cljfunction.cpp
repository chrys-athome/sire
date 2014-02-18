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

#include <QElapsedTimer>

#include "cljfunction.h"
#include "gridinfo.h"

#include "SireMaths/multidouble.h"

#include "SireVol/cartesian.h"
#include "SireVol/periodicbox.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace SireMM;
using namespace SireVol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireID;
using namespace SireStream;

/////////
///////// Implementation of CLJFunction
/////////

static const RegisterMetaType<CLJFunction> r_cljfunc(MAGIC_ONLY, CLJFunction::typeName());

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJFunction &cljfunc)
{
    writeHeader(ds, r_cljfunc, 1);
    
    SharedDataStream sds(ds);
    
    sds << cljfunc.spce << cljfunc.use_arithmetic;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFunction &cljfunc)
{
    VersionID v = readHeader(ds, r_cljfunc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cljfunc.spce >> cljfunc.use_arithmetic;
        cljfunc.extractDetailsFromSpace();
    }
    else
        throw version_error(v, "1", r_cljfunc, CODELOC);
    
    return ds;
}

/** Constructor. By default we will use vacuum boundary conditions with
    arithmetic combining rules */
CLJFunction::CLJFunction() : Property(), use_arithmetic(true), use_box(false)
{}

void CLJFunction::extractDetailsFromRules(SireMM::CLJFunction::COMBINING_RULES rules)
{
    switch (rules)
    {
        case ARITHMETIC:
            use_arithmetic = true;
            break;
        case GEOMETRIC:
            use_arithmetic = false;
            break;
        default:
            throw SireError::program_bug( QObject::tr(
                        "Cannot understand the passed combining rules (%1)")
                            .arg(rules), CODELOC );
    }
}

/** Construct, using vacuum boundary conditions, but specifying the combining rules */
CLJFunction::CLJFunction(COMBINING_RULES combining_rules)
            : Property(), use_arithmetic(true), use_box(false)
{
    extractDetailsFromRules(combining_rules);
}

void CLJFunction::extractDetailsFromSpace()
{
    if (spce.isNull())
    {
        use_box = false;
        box_dimensions = Vector(0);
    }
    else if (spce.read().isA<PeriodicBox>())
    {
        use_box = true;
        box_dimensions = spce.read().asA<PeriodicBox>().dimensions();
    }
    else if (spce.read().isA<Cartesian>())
    {
        use_box = false;
        box_dimensions = Vector(0);
    }
    else
        throw SireError::unsupported( QObject::tr(
                "CLJFunction-based forcefields currently only support using either "
                "periodic (cubic) boundary conditions, or vacuum boundary conditions. "
                "They are not compatible with the passed space \"%1\".")
                    .arg(spce.read().toString()), CODELOC );
}

/** Construct, using arithmetic combining rules, but specifying the space */
CLJFunction::CLJFunction(const Space &space)
            : Property(), spce(space), use_arithmetic(true), use_box(false)
{
    extractDetailsFromSpace();
}

/** Construct, specifying both the combining rules and simulation space */
CLJFunction::CLJFunction(const Space &space, COMBINING_RULES combining_rules)
            : Property(), spce(space), use_arithmetic(true), use_box(false)
{
    extractDetailsFromSpace();
    extractDetailsFromRules(combining_rules);
}

/** Copy constructor */
CLJFunction::CLJFunction(const CLJFunction &other)
            : Property(other), spce(other.spce), box_dimensions(other.box_dimensions),
              use_arithmetic(other.use_arithmetic), use_box(other.use_box)
{}

/** Destructor */
CLJFunction::~CLJFunction()
{}

/** Copy assignment operator */
CLJFunction& CLJFunction::operator=(const CLJFunction &other)
{
    if (this != &other)
    {
        spce = other.spce;
        box_dimensions = other.box_dimensions;
        use_arithmetic = other.use_arithmetic;
        use_box = other.use_box;
        Property::operator=(other);
    }
    
    return *this;
}

const char* CLJFunction::typeName()
{
    return "SireMM::CLJFunction";
}

/** Comparison operator */
bool CLJFunction::operator==(const CLJFunction &other) const
{
    return use_arithmetic == other.use_arithmetic and Property::operator==(other);
}

/** Tell the function to use arithmetic combining rules for LJ parameters */
void CLJFunction::setArithmeticCombiningRules(bool on)
{
    use_arithmetic = on;
}

/** Tell the function to use geometric combining rules for LJ parameters */
void CLJFunction::setGeometricCombiningRules(bool on)
{
    use_arithmetic = not on;
}

/** Return the combining rules used by the function */
SireMM::CLJFunction::COMBINING_RULES CLJFunction::combiningRules() const
{
    if (use_arithmetic)
        return ARITHMETIC;
    else
        return GEOMETRIC;
}

/** Return whether or not this function uses a softened (soft-core) potential */
bool CLJFunction::isSoftened() const
{
    return false;
}

/** Set the combining rules used by the function */
void CLJFunction::setCombiningRules(COMBINING_RULES rules)
{
    extractDetailsFromRules(rules);
}

/** Return whether or not this function uses a cutoff */
bool CLJFunction::hasCutoff() const
{
    return false;
}

/** Return the coulomb cutoff if this function uses one */
Length CLJFunction::coulombCutoff() const
{
    return Length( std::numeric_limits<double>::max() );
}

/** Return the LJ cutoff if this function uses one */
Length CLJFunction::ljCutoff() const
{
    return Length( std::numeric_limits<double>::max() );
}

/** Set the coulomb and LJ cutoff to 'distance', if this function
    has a cutoff */
void CLJFunction::setCutoff(Length distance)
{
    setCoulombCutoff(distance);
    setLJCutoff(distance);
}

/** Set the coulomb and LJ cutoff to the specified values, if this function
    has a cutoff */
void CLJFunction::setCutoff(Length coulomb_cutoff, Length lj_cutoff)
{
    setCoulombCutoff(coulomb_cutoff);
    setLJCutoff(lj_cutoff);
}

/** Set the coulomb cutoff if this function has a cutoff */
void CLJFunction::setCoulombCutoff(Length)
{
    throw SireError::unsupported( QObject::tr( "The CLJFunction \"%1\" does not "
                    "support use of a cutoff, so one cannot be set.")
                            .arg(this->toString()), CODELOC );
}

/** Set the LJ cutoff if this function has a cutoff */
void CLJFunction::setLJCutoff(Length)
{
    throw SireError::unsupported( QObject::tr( "The CLJFunction \"%1\" does not "
                    "support use of a cutoff, so one cannot be set.")
                            .arg(this->toString()), CODELOC );
}

/** Return the space represented by the function */
const Space& CLJFunction::space() const
{
    return spce.read();
}

/** Return whether or not the space of the function is periodic */
bool CLJFunction::isPeriodic() const
{
    return use_box;
}

/** Set the space used by the function */
void CLJFunction::setSpace(const Space &space)
{
    SpacePtr old_space = spce;
    
    try
    {
        spce = space;
        extractDetailsFromSpace();
    }
    catch(...)
    {
        spce = old_space;
        throw;
    }
}

/** Return whether or not arithmetic combining rules are used */
bool CLJFunction::usingArithmeticCombiningRules() const
{
    return use_arithmetic;
}

/** Return whether or not geometric combining rules are used */
bool CLJFunction::usingGeometricCombiningRules() const
{
    return not use_arithmetic;
}

/** Return whether or not this function supports calculating potentials on grids */
bool CLJFunction::supportsGridCalculation() const
{
    return false;
}

/** Dummy function that needs to be overridden to support grid calculations */
void CLJFunction::calcBoxGrid(const CLJAtoms &atoms, const GridInfo &gridinfo,
                              const int start, const int end, float *gridpot) const
{
    for (int i=start; i<end; ++i)
    {
        gridpot[i] = 0;
    }
}

/** Dummy function that needs to be overridden to support grid calculations */
void CLJFunction::calcVacGrid(const CLJAtoms &atoms, const GridInfo &gridinfo,
                              const int start, const int end, float *gridpot) const
{
    for (int i=start; i<end; ++i)
    {
        gridpot[i] = 0;
    }
}

namespace SireMM
{
    namespace detail
    {
        class CLJGridCalculator
        {
        public:
            CLJGridCalculator()
                : atoms(0), grid_info(0), cljfunc(0), gridpot(0), use_box(false)
            {}
            
            CLJGridCalculator(const CLJAtoms &_atoms, const GridInfo &_grid_info,
                              const CLJFunction &_cljfunc, float *_gridpot)
                : atoms(&_atoms), grid_info(&_grid_info), cljfunc(&_cljfunc),
                  gridpot(_gridpot), use_box(_cljfunc.use_box)
            {}
            
            ~CLJGridCalculator()
            {}
            
            void operator()(const tbb::blocked_range<int> &range) const
            {
                if (use_box)
                {
                    cljfunc->calcBoxGrid(*atoms, *grid_info,
                                         range.begin(), range.end(),
                                         &(gridpot[range.begin()]));
                }
                else
                {
                    cljfunc->calcVacGrid(*atoms, *grid_info,
                                         range.begin(), range.end(),
                                         &(gridpot[range.begin()]));
                }
            }
            
        private:
            const CLJAtoms* const atoms;
            const GridInfo* const grid_info;
            const CLJFunction* const cljfunc;
            float *gridpot;
            const bool use_box;
        };
    }
}

/** Return the potential on the described grid of the passed atoms using
    this function. This returns an empty grid if this function doesn't support
    grid calculations */
QVector<float> CLJFunction::calculate(const CLJAtoms &atoms, const GridInfo &gridinfo) const
{
    QElapsedTimer t;
    t.start();

    QVector<float> gridpot( gridinfo.nPoints(), 0.0 );
    
    if (this->supportsGridCalculation() and not gridinfo.isEmpty())
    {
        SireMM::detail::CLJGridCalculator calc(atoms, gridinfo, *this, gridpot.data());
        tbb::parallel_for(tbb::blocked_range<int>(0,gridinfo.nPoints(),4096), calc);
    }

    qint64 ns = t.nsecsElapsed();
    qDebug() << "Building the grid took" << (0.000001*ns) << "ms";
    
    double sum = 0;
    for (int i=0; i<gridinfo.nPoints(); ++i)
    {
        sum += gridpot.constData()[i];
    }
    qDebug() << "Sum of grid potential is" << sum;
    
    return gridpot;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcVacEnergyAri(atoms, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                            float min_distance) const
{
    double cnrg, ljnrg;
    calcVacEnergyAri(atoms0, atoms1, cnrg, ljnrg, min_distance);
    return cnrg;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcVacCoulombEnergyGeo(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcVacEnergyGeo(atoms, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcVacCoulombEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                            float min_distance) const
{
    double cnrg, ljnrg;
    calcVacEnergyGeo(atoms0, atoms1, cnrg, ljnrg, min_distance);
    return cnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcVacLJEnergyAri(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcVacEnergyAri(atoms, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcVacLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                       float min_distance) const
{
    double cnrg, ljnrg;
    calcVacEnergyAri(atoms0, atoms1, cnrg, ljnrg, min_distance);
    return ljnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcVacLJEnergyGeo(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcVacEnergyGeo(atoms, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcVacLJEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                       float min_distance) const
{
    double cnrg, ljnrg;
    calcVacEnergyGeo(atoms0, atoms1, cnrg, ljnrg, min_distance);
    return ljnrg;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcBoxCoulombEnergyAri(const CLJAtoms &atoms,
                                            const Vector &box_dimensions) const
{
    double cnrg, ljnrg;
    calcBoxEnergyAri(atoms, box_dimensions, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcBoxCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                            const Vector &box_dimensions,
                                            float min_distance) const
{
    double cnrg, ljnrg;
    calcBoxEnergyAri(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
    return cnrg;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcBoxCoulombEnergyGeo(const CLJAtoms &atoms,
                                            const Vector &box_dimensions) const
{
    double cnrg, ljnrg;
    calcBoxEnergyGeo(atoms, box_dimensions, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcBoxCoulombEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                            const Vector &box_dimensions,
                                            float min_distance) const
{
    double cnrg, ljnrg;
    calcBoxEnergyGeo(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
    return cnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcBoxLJEnergyAri(const CLJAtoms &atoms,
                                       const Vector &box_dimensions) const
{
    double cnrg, ljnrg;
    calcBoxEnergyAri(atoms, box_dimensions, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcBoxLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                       const Vector &box_dimensions, float min_distance) const
{
    double cnrg, ljnrg;
    calcBoxEnergyAri(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
    return ljnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcBoxLJEnergyGeo(const CLJAtoms &atoms,
                                       const Vector &box_dimensions) const
{
    double cnrg, ljnrg;
    calcBoxEnergyGeo(atoms, box_dimensions, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcBoxLJEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                       const Vector &box_dimensions,
                                       float min_distance) const
{
    double cnrg, ljnrg;
    calcBoxEnergyGeo(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
    return ljnrg;
}

void CLJFunction::operator()(const CLJAtoms &atoms,
                             double &cnrg, double &ljnrg) const
{
    if (atoms.isEmpty())
    {
        cnrg = 0;
        ljnrg = 0;
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                this->calcBoxEnergyAri(atoms, box_dimensions, cnrg, ljnrg);
            else
                this->calcVacEnergyAri(atoms, cnrg, ljnrg);
        }
        else
        {
            if (use_box)
                this->calcBoxEnergyGeo(atoms, box_dimensions, cnrg, ljnrg);
            else
                this->calcVacEnergyGeo(atoms, cnrg, ljnrg);
        }
    }
}

void CLJFunction::operator()(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                             double &cnrg, double &ljnrg, float min_distance) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        cnrg = 0;
        ljnrg = 0;
        return;
    }
    else if (atoms0.count() > atoms1.count())
    {
        if (use_arithmetic)
        {
            if (use_box)
                this->calcBoxEnergyAri(atoms1, atoms0, box_dimensions, cnrg, ljnrg, min_distance);
            else
                this->calcVacEnergyAri(atoms1, atoms0, cnrg, ljnrg, min_distance);
        }
        else
        {
            if (use_box)
                this->calcBoxEnergyGeo(atoms1, atoms0, box_dimensions, cnrg, ljnrg, min_distance);
            else
                this->calcVacEnergyGeo(atoms1, atoms0, cnrg, ljnrg, min_distance);
        }
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                this->calcBoxEnergyAri(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
            else
                this->calcVacEnergyAri(atoms0, atoms1, cnrg, ljnrg, min_distance);
        }
        else
        {
            if (use_box)
                this->calcBoxEnergyGeo(atoms0, atoms1, box_dimensions, cnrg, ljnrg, min_distance);
            else
                this->calcVacEnergyGeo(atoms0, atoms1, cnrg, ljnrg, min_distance);
        }
    }
}

/** Return the total energy between 'atoms', returning the coulomb part in 'cnrg'
    and the LJ part in 'ljnrg' */
void CLJFunction::total(const CLJAtoms &atoms, double &cnrg, double &ljnrg) const
{
    return this->operator()(atoms, cnrg, ljnrg);
}

/** Return the total energy between 'atoms0' and 'atoms1', returning the coulomb part in 'cnrg'
    and the LJ part in 'ljnrg' */
void CLJFunction::total(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                        double &cnrg, double &ljnrg, float min_distance) const
{
    return this->operator()(atoms0, atoms1, cnrg, ljnrg, min_distance);
}

boost::tuple<double,double> CLJFunction::calculate(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    this->operator()(atoms, cnrg, ljnrg);
    return boost::tuple<double,double>(cnrg, ljnrg);
}

boost::tuple<double,double> CLJFunction::calculate(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                                   float min_distance) const
{
    double cnrg, ljnrg;
    this->operator()(atoms0, atoms1, cnrg, ljnrg, min_distance);
    return boost::tuple<double,double>(cnrg, ljnrg);
}

/** Return the coulomb energy between the atoms in 'atoms' */
double CLJFunction::coulomb(const CLJAtoms &atoms) const
{
    if (atoms.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxCoulombEnergyAri(atoms, box_dimensions);
            else
                return this->calcVacCoulombEnergyAri(atoms);
        }
        else
        {
            if (use_box)
                return this->calcBoxCoulombEnergyGeo(atoms, box_dimensions);
            else
                return this->calcVacCoulombEnergyGeo(atoms);
        }
    }
}

/** Return the coulomb energy between the atoms in 'atoms0' and in 'atoms1' */
double CLJFunction::coulomb(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                            float min_distance) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        return 0;
    }
    else if (atoms0.count() > atoms1.count())
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxCoulombEnergyAri(atoms1, atoms0, box_dimensions, min_distance);
            else
                return this->calcVacCoulombEnergyAri(atoms1, atoms0, min_distance);
        }
        else
        {
            if (use_box)
                return this->calcBoxCoulombEnergyGeo(atoms1, atoms0, box_dimensions, min_distance);
            else
                return this->calcVacCoulombEnergyGeo(atoms1, atoms0, min_distance);
        }
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxCoulombEnergyAri(atoms0, atoms1, box_dimensions, min_distance);
            else
                return this->calcVacCoulombEnergyAri(atoms0, atoms1, min_distance);
        }
        else
        {
            if (use_box)
                return this->calcBoxCoulombEnergyGeo(atoms0, atoms1, box_dimensions, min_distance);
            else
                return this->calcVacCoulombEnergyGeo(atoms0, atoms1, min_distance);
        }
    }
}

/** Return the LJ energy between the atoms in 'atoms' */
double CLJFunction::lj(const CLJAtoms &atoms) const
{
    if (atoms.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxLJEnergyAri(atoms, box_dimensions);
            else
                return this->calcVacLJEnergyAri(atoms);
        }
        else
        {
            if (use_box)
                return this->calcBoxLJEnergyGeo(atoms, box_dimensions);
            else
                return this->calcVacLJEnergyGeo(atoms);
        }
    }
}

/** Return the LJ energy between the atoms in 'atoms0' and in 'atoms1' */
double CLJFunction::lj(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                       float min_distance) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        return 0;
    }
    else if (atoms0.count() > atoms1.count())
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxLJEnergyAri(atoms1, atoms0, box_dimensions, min_distance);
            else
                return this->calcVacLJEnergyAri(atoms1, atoms0, min_distance);
        }
        else
        {
            if (use_box)
                return this->calcBoxLJEnergyGeo(atoms1, atoms0, box_dimensions, min_distance);
            else
                return this->calcVacLJEnergyGeo(atoms1, atoms0, min_distance);
        }
    }
    else
    {
        if (use_arithmetic)
        {
            if (use_box)
                return this->calcBoxLJEnergyAri(atoms0, atoms1, box_dimensions, min_distance);
            else
                return this->calcVacLJEnergyAri(atoms0, atoms1, min_distance);
        }
        else
        {
            if (use_box)
                return this->calcBoxLJEnergyGeo(atoms0, atoms1, box_dimensions, min_distance);
            else
                return this->calcVacLJEnergyGeo(atoms0, atoms1, min_distance);
        }
    }
}

/////////
///////// Implementation of CLJCutoffFunction
/////////

static const RegisterMetaType<CLJCutoffFunction> r_cutoff(MAGIC_ONLY,
                                                          CLJCutoffFunction::typeName());

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJCutoffFunction &func)
{
    writeHeader(ds, r_cutoff, 1);
    
    ds << func.coul_cutoff << func.lj_cutoff
       << static_cast<const CLJFunction&>(func);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJCutoffFunction &func)
{
    VersionID v = readHeader(ds, r_cutoff);
    
    if (v == 1)
    {
        ds >> func.coul_cutoff >> func.lj_cutoff
           >> static_cast<CLJFunction&>(func);
    }
    else
        throw version_error(v, "1", r_cutoff, CODELOC);
    
    return ds;
}

/** Default constructor - uses a cutoff of 10 angstrom */
CLJCutoffFunction::CLJCutoffFunction()
                  : CLJFunction(), coul_cutoff(10), lj_cutoff(10)
{}

void CLJCutoffFunction::pvt_setCutoff(Length coulomb, Length lj)
{
    if (coulomb.value() < 0)
    {
        coul_cutoff = 0;
    }
    else if (coulomb.value() > std::numeric_limits<float>::max())
    {
        coul_cutoff = std::numeric_limits<float>::max();
    }
    else
    {
        coul_cutoff = coulomb.value();
    }

    if (lj.value() < 0)
    {
        lj_cutoff = 0;
    }
    else if (lj.value() > std::numeric_limits<float>::max())
    {
        lj_cutoff = std::numeric_limits<float>::max();
    }
    else
    {
        lj_cutoff = lj.value();
    }
}

/** Construct, specifying the cutoff */
CLJCutoffFunction::CLJCutoffFunction(Length cutoff)
                  : CLJFunction(), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(cutoff, cutoff);
}

/** Construct, specifying the coulomb and LJ cutoffs */
CLJCutoffFunction::CLJCutoffFunction(Length coulomb, Length lj)
                  : CLJFunction(), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(coulomb, lj);
}

CLJCutoffFunction::CLJCutoffFunction(const Space &space, Length cutoff)
                  : CLJFunction(space), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(cutoff, cutoff);
}

CLJCutoffFunction::CLJCutoffFunction(const Space &space, Length coulomb, Length lj)
                  : CLJFunction(space), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(coulomb, lj);
}

CLJCutoffFunction::CLJCutoffFunction(Length cutoff, COMBINING_RULES combining_rules)
                  : CLJFunction(combining_rules), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(cutoff, cutoff);
}

CLJCutoffFunction::CLJCutoffFunction(Length coulomb, Length lj, COMBINING_RULES combining_rules)
                  : CLJFunction(combining_rules), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(coulomb, lj);
}

CLJCutoffFunction::CLJCutoffFunction(const Space &space, COMBINING_RULES combining_rules)
                  : CLJFunction(space, combining_rules), coul_cutoff(10), lj_cutoff(10)
{}

CLJCutoffFunction::CLJCutoffFunction(const Space &space, Length cutoff,
                                     COMBINING_RULES combining_rules)
                  : CLJFunction(space, combining_rules), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(cutoff, cutoff);
}

CLJCutoffFunction::CLJCutoffFunction(const Space &space, Length coulomb, Length lj,
                                     COMBINING_RULES combining_rules)
                  : CLJFunction(space, combining_rules), coul_cutoff(10), lj_cutoff(10)
{
    pvt_setCutoff(coulomb, lj);
}

/** Copy constructor */
CLJCutoffFunction::CLJCutoffFunction(const CLJCutoffFunction &other)
                  : CLJFunction(other), coul_cutoff(other.coul_cutoff), lj_cutoff(other.lj_cutoff)
{}

/** Destructor */
CLJCutoffFunction::~CLJCutoffFunction()
{}

/** Copy assignment operator */
CLJCutoffFunction& CLJCutoffFunction::operator=(const CLJCutoffFunction &other)
{
    if (this != &other)
    {
        coul_cutoff = other.coul_cutoff;
        lj_cutoff = other.lj_cutoff;
        CLJFunction::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool CLJCutoffFunction::operator==(const CLJCutoffFunction &other) const
{
    return coul_cutoff == other.coul_cutoff and lj_cutoff == other.lj_cutoff and
           CLJFunction::operator==(other);
}

const char* CLJCutoffFunction::typeName()
{
    return "SireMM::CLJCutoffFunction";
}

/** Return whether or not this function has a cutoff */
bool CLJCutoffFunction::hasCutoff() const
{
    return true;
}

/** Return the coulomb cutoff distance */
Length CLJCutoffFunction::coulombCutoff() const
{
    return Length(coul_cutoff);
}

/** Return the LJ cutoff distance */
Length CLJCutoffFunction::ljCutoff() const
{
    return Length(lj_cutoff);
}

/** Set the coulomb and LJ cutoff distances to 'distance' */
void CLJCutoffFunction::setCutoff(Length distance)
{
    pvt_setCutoff(distance, distance);
}

/** Set the coulomb and LJ cutoff distances to the specified values */
void CLJCutoffFunction::setCutoff(Length coulomb, Length lj)
{
    pvt_setCutoff(coulomb, lj);
}

/** Set the coulomb cutoff to the specified distance */
void CLJCutoffFunction::setCoulombCutoff(Length distance)
{
    pvt_setCutoff(distance, ljCutoff());
}

/** Set the LJ cutoff to the specified distance */
void CLJCutoffFunction::setLJCutoff(Length distance)
{
    pvt_setCutoff(coulombCutoff(), distance);
}

/////////
///////// Implementation of CLJIntraFunction
/////////

static const RegisterMetaType<CLJIntraFunction> r_intra(MAGIC_ONLY, CLJIntraFunction::typeName());

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJIntraFunction &func)
{
    writeHeader(ds, r_intra, 1);
    
    SharedDataStream sds(ds);
    
    sds << func.cty
        << static_cast<const CLJCutoffFunction&>(func);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJIntraFunction &func)
{
    VersionID v = readHeader(ds, r_intra);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        func.bond_matrix.clear();
        func.cty = Connectivity();
        
        Connectivity connectivity;
        
        sds >> connectivity
            >> static_cast<CLJCutoffFunction&>(func);
        
        func.setConnectivity(connectivity);
    }
    else
        throw version_error(v, "1", r_intra, CODELOC);
    
    return ds;
}


CLJIntraFunction::CLJIntraFunction() : CLJCutoffFunction()
{}

CLJIntraFunction::CLJIntraFunction(Length cutoff) : CLJCutoffFunction(cutoff)
{}

CLJIntraFunction::CLJIntraFunction(Length coul_cutoff, Length lj_cutoff)
                 : CLJCutoffFunction(coul_cutoff, lj_cutoff)
{}

CLJIntraFunction::CLJIntraFunction(const Space &space, Length cutoff)
                 : CLJCutoffFunction(space, cutoff)
{}

CLJIntraFunction::CLJIntraFunction(const Space &space,
                                   Length coul_cutoff, Length lj_cutoff)
                 : CLJCutoffFunction(space, coul_cutoff, lj_cutoff)
{}

CLJIntraFunction::CLJIntraFunction(Length cutoff, COMBINING_RULES combining_rules)
                 : CLJCutoffFunction(cutoff, combining_rules)
{}

CLJIntraFunction::CLJIntraFunction(Length coul_cutoff, Length lj_cutoff,
                                   COMBINING_RULES combining_rules)
                 : CLJCutoffFunction(coul_cutoff, lj_cutoff, combining_rules)
{}

CLJIntraFunction::CLJIntraFunction(const Space &space, COMBINING_RULES combining_rules)
                 : CLJCutoffFunction(space, combining_rules)
{}

CLJIntraFunction::CLJIntraFunction(const Space &space, Length cutoff,
                                   COMBINING_RULES combining_rules)
                 : CLJCutoffFunction(space, cutoff, combining_rules)
{}

CLJIntraFunction::CLJIntraFunction(const Space &space, Length coul_cutoff,
                                   Length lj_cutoff,
                                   COMBINING_RULES combining_rules)
                 : CLJCutoffFunction(space, coul_cutoff, lj_cutoff, combining_rules)
{}

/** Copy constructor */
CLJIntraFunction::CLJIntraFunction(const CLJIntraFunction &other)
                 : CLJCutoffFunction(other), cty(other.cty), bond_matrix(other.bond_matrix)
{}

/** Destructor */
CLJIntraFunction::~CLJIntraFunction()
{}

const char* CLJIntraFunction::typeName()
{
    return "SireMM::CLJIntraFunction";
}

/** Copy assignment operator */
CLJIntraFunction& CLJIntraFunction::operator=(const CLJIntraFunction &other)
{
    if (this != &other)
    {
        cty = other.cty;
        bond_matrix = other.bond_matrix;
        CLJCutoffFunction::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool CLJIntraFunction::operator==(const CLJIntraFunction &other) const
{
    return cty == other.cty and CLJCutoffFunction::operator==(other);
}

/** Return the connectivity used to find the non-bonded pairs */
const Connectivity& CLJIntraFunction::connectivity() const
{
    return cty;
}

/** Set the connectivity used to find the non-bonded pairs */
void CLJIntraFunction::setConnectivity(const Connectivity &c)
{
    if (cty != c)
    {
        QElapsedTimer t;
        t.start();
    
        cty = c;
        bond_matrix = cty.getBondMatrix(1,4);

        quint64 ns1 = t.nsecsElapsed();
        t.start();

        if (bond_matrix.count() > 0)
        {
            //we now need to pad this with zeroes, as we use AtomIdx(0) to mean a dummy atom
            bond_matrix.prepend( QVector<bool>(bond_matrix.count() + 1, false) );

            bond_matrix[0].squeeze();
            
            for (int i=1; i<bond_matrix.count(); ++i)
            {
                bond_matrix[i].prepend(false);
                bond_matrix[i].squeeze();
            }
            
            bond_matrix.squeeze();
        }
        
        quint64 ns2 = t.nsecsElapsed();
        
        qDebug() << "GETTING MATRIX TOOK" << (0.000001*(ns1+ns2)) << "ms, "
                 << (0.000001*ns1) << "," << (0.000001*ns2);
    }
}

/** Set the connectivity by copying the specified property from the passed molecule */
void CLJIntraFunction::setConnectivity(const MoleculeView &molecule, const PropertyMap &map)
{
    setConnectivity( molecule.data().property( map["connectivity"] ).asA<Connectivity>() );
}

/** Return whether or not there are no bonded pairs between the atoms in 'ids0' and 'ids1' */
bool CLJIntraFunction::isNotBonded(const QVector<MultiInt> &ids0,
                                   const QVector<MultiInt> &ids1) const
{
    const int nats0 = ids0.count();
    const int nats1 = ids1.count();
    
    const MultiInt *aid0 = ids0.constData();
    const MultiInt *aid1 = ids1.constData();
    
    for (int i=0; i<nats0; ++i)
    {
        const MultiInt &id0 = aid0[i];
    
        for (int ii=0; ii<MultiInt::count(); ++ii)
        {
            const bool *row = bond_matrix.constData()[ id0[ii] ].constData();
            
            for (int j=0; j<nats1; ++j)
            {
                const MultiInt &id1 = aid1[j];
                
                for (int jj=0; jj<MultiInt::count(); ++jj)
                {
                    if (row[id1[jj]])
                        return false;
                }
            }
        }
    
        /*for (int j=0; j<nats1; ++j)
        {
            const MultiInt &id1 = aid1[j];
            
            for (int ii=0; ii<MultiInt::count(); ++ii)
            {
                const QVector<bool> &row = bond_matrix.constData()[ id0[ii] ];
            
                for (int jj=0; jj<MultiInt::count(); ++jj)
                {
                    if (row.constData()[ id1[jj] ])
                    {
                        return false;
                    }
                }
            }
        }*/
    }
    
    return true;
}

/////////
///////// Implementation of CLJSoftFunction
/////////

static const RegisterMetaType<CLJSoftFunction> r_soft(MAGIC_ONLY, CLJSoftFunction::typeName());

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJSoftFunction &func)
{
    writeHeader(ds, r_soft, 1);
    
    ds << func.alpha_value << func.shift_delta << func.coulomb_power
       << static_cast<const CLJCutoffFunction&>(func);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJSoftFunction &func)
{
    VersionID v = readHeader(ds, r_soft);
    
    if (v == 1)
    {
        ds >> func.alpha_value >> func.shift_delta >> func.coulomb_power
           >> static_cast<CLJCutoffFunction&>(func);
    }
    else
        throw version_error(v, "1", r_soft, CODELOC);
    
    return ds;
}

/** Constructor */
CLJSoftFunction::CLJSoftFunction()
                : CLJCutoffFunction(), alpha_value(0), shift_delta(0), coulomb_power(1)
{}

/** Copy constructor */
CLJSoftFunction::CLJSoftFunction(const CLJSoftFunction &other)
                : CLJCutoffFunction(other), alpha_value(other.alpha_value),
                  shift_delta(other.shift_delta), coulomb_power(other.coulomb_power)
{}

/** Destructor */
CLJSoftFunction::~CLJSoftFunction()
{}

/** Copy assignment operator */
CLJSoftFunction& CLJSoftFunction::operator=(const CLJSoftFunction &other)
{
    if (this != &other)
    {
        alpha_value = other.alpha_value;
        shift_delta = other.shift_delta;
        coulomb_power = other.coulomb_power;
        CLJCutoffFunction::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool CLJSoftFunction::operator==(const CLJSoftFunction &other) const
{
    return alpha_value == other.alpha_value and
           shift_delta == other.shift_delta and
           coulomb_power == other.coulomb_power and
           CLJCutoffFunction::operator==(other);
}

const char* CLJSoftFunction::typeName()
{
    return "SireMM::CLJSoftFunction";
}

/** Return whether or not this is a softened function */
bool CLJSoftFunction::isSoftened() const
{
    return true;
}

/** Return the soft-core alpha value. A value of 0 is a completely hard
    potential, while increasing values of alpha will increasingly soften 
    the potential */
float CLJSoftFunction::alpha() const
{
    return alpha_value;
}

/** Return the soft-core shift_delta parameter. This is used to soften
    the LJ interactions */
float CLJSoftFunction::shiftDelta() const
{
    return shift_delta;
}

/** Return the soft-core coulomb_power parameter. This is used to soften
    the electrostatic interactions */
float CLJSoftFunction::coulombPower() const
{
    return coulomb_power;
}

void CLJSoftFunction::pvt_set(float alpha, float shift, float power)
{
    if (alpha < 0)
        alpha = 0;
    
    if (shift < 0)
        shift = 0;

    if (alpha > 1)
    {
        if ( power != int(power) )
            throw SireError::incompatible_error( QObject::tr(
                    "You cannot have a soft-core function where alpha > 1 and "
                    "you have a non-integer coulomb power (alpha = %1, coulomb power = %2)")
                        .arg(alpha).arg(power), CODELOC );
    }
    
    alpha_value = alpha;
    shift_delta = shift;
    coulomb_power = power;
}

/** Set the soft-core alpha parameter */
void CLJSoftFunction::setAlpha(float alp)
{
    pvt_set(alp, shiftDelta(), coulombPower());
}

/** Set the soft-core shift delta parameter */
void CLJSoftFunction::setShiftDelta(float shift)
{
    pvt_set(alpha(), shift, coulombPower());
}

/** Set the soft-core coulomb power parameter */
void CLJSoftFunction::setCoulombPower(float power)
{
    pvt_set(alpha(), shiftDelta(), power);
}
